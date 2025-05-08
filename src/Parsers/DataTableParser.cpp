#include "../../include/Spectrotool/Parsers/DataTableParser.hpp"
#include "../utils.hpp"
#include <OpenXLSX.hpp>
#include <regex>

namespace Spectrotool {

    void addCompound(Compound &compound, const OpenXLSX::XLCell &row) {
        CompoundValue value;
        value.name = getXLValue<std::string>(row.offset(0, 2));
        value.id = getXLValue<std::string>(row.offset(0, 3));
        value.rt = getXLValue<double>(row.offset(0, 4));
        value.area = getXLValue<double>(row.offset(0, 5));
        value.isArea = getXLValue<double>(row.offset(0, 6));
        value.sDivN = getXLValue<double>(row.offset(0, 7));

        compound.addValue(std::move(value));
    }

    std::string validCompound(const std::string& potentialMatch, ParserMessages& msgs, const DataTableDesc& desc) {
        // check if it is a compound entry and not a random string
        std::regex pattern(R"(^Compound\s+\d+:\s+(.+)$)");
        std::smatch match;
        if (!std::regex_match(potentialMatch, match, pattern)) {
            return ""; // empty string
        }
        std::string compoundName = match[1];
        sanitizeString(compoundName);

        // run the filter
        for (const std::string& filter: desc.excludeCompoundFilter) {
            if (compoundName.find(filter) != std::string::npos) {
                return "";
            }
        }

        return compoundName;
    }

    void parseWorkSheet(const OpenXLSX::XLWorksheet& sheet, const DataTableDesc& desc, DataTable& table, ParserMessages& msgs, std::unordered_set<std::string>& compoundNames) {
        Compound* current = nullptr;

        // Specify a quadratic region that is 1 column wide and as long as the sheet
        const auto range = sheet.range(OpenXLSX::XLCellReference("A1"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));

        for (const auto& row : range) {
            switch (row.value().type()) {
                case OpenXLSX::XLValueType::String: {
                    // Check if it is a valid compound
                    std::string value = row.value();
                    const std::string formattedName = validCompound(value, msgs, desc);
                    if (formattedName.empty()) {
                        current = nullptr;
                        continue;
                    }
                    if (compoundNames.find(formattedName) == compoundNames.end()) {
                        compoundNames.insert(formattedName);
                    }
                    else {
                        msgs.addWarning("Duplicate compound: " + formattedName, row.cellReference().address(), sheet.name());
                    }
                    Compound c(formattedName);
                    table.addCompound(std::move(c));
                    current = &table.getCompounds().back();
                    break;
                }
                case OpenXLSX::XLValueType::Integer: {
                    if (current == nullptr) {
                        continue;
                    }
                    addCompound(*current, row);
                }
                default: break;
            }
        }
    }

    ParserMessages parseDataTable(const DataTableDesc& desc, DataTable& table) {
        ParserMessages msgs;
        if (!fs::exists(desc.filePath)) {
            msgs.addError("File " + desc.filePath.string() + " does not exist!");
            return msgs;
        }
        OpenXLSX::XLDocument document;
        document.open(desc.filePath.string());

        std::unordered_set<std::string> compoundNames;
        for (const auto& sheetName: document.workbook().sheetNames()) {
            const OpenXLSX::XLWorksheet& sheet = document.workbook().worksheet(sheetName);
            if (sheet.rowCount() == 0 || sheet.columnCount() == 0) {
                msgs.addWarning("Sheet is empty!", "A1", sheetName);
            }
            else {
                try {
                    parseWorkSheet(sheet, desc, table, msgs, compoundNames);
                } catch (std::exception& e) {
                    msgs.addError("Invalid Parse!", "UNKNOWN", sheetName);
                }
            }
        }

        return msgs;
    }

    void formatHeader(const OpenXLSX::XLWorksheet &sheet){
        sheet.cell("A1").value() = "Name";
        sheet.cell("B1").value() = "ID";
        sheet.cell("C1").value() = "RT";
        sheet.cell("D1").value() = "Area";
        sheet.cell("E1").value() = "IS Area";
        sheet.cell("F1").value() = "S/N";

        sheet.cell("G1").value() = "Weight";
        sheet.cell("H1").value() = "Matrix";

        sheet.cell("I1").value() = "CorrectedISArea";
        sheet.cell("J1").value() = "uncorrectedConcentration (pg/g)";
        sheet.cell("K1").value() = "uncorrectedConcentration (ug/kg)";
        sheet.cell("L1").value() = "correctedConcentration";
    }

    void exportCompound(const Compound& compound, OpenXLSX::XLWorksheet &sheet) {
        std::size_t row = 2;
        for (const auto& compoundValue: compound.getValues()) {
            setXLValue(sheet, "A" + std::to_string(row), compoundValue.name);
            setXLValue(sheet, "B" + std::to_string(row), compoundValue.id);
            setXLValue(sheet, "C" + std::to_string(row), compoundValue.rt);
            setXLValue(sheet, "D" + std::to_string(row), compoundValue.area);
            setXLValue(sheet, "E" + std::to_string(row), compoundValue.isArea);
            setXLValue(sheet, "F" + std::to_string(row), compoundValue.sDivN);

            setXLValue(sheet, "G" + std::to_string(row), compoundValue.weight);
            setXLValue(sheet, "H" + std::to_string(row), compoundValue.matrix);

            setXLValue(sheet, "I" + std::to_string(row), compoundValue.correctedISArea);
            setXLValue(sheet, "J" + std::to_string(row), compoundValue.uncorrectedConcentration_pgg);
            setXLValue(sheet, "K" + std::to_string(row), compoundValue.uncorrectedConcentration_microgkg);
            setXLValue(sheet, "L" + std::to_string(row), compoundValue.correctedConcentration);
            row++;
        }
    }

    void exportDataTable(const DataTable& table, const fs::path& filePath) {
        OpenXLSX::XLDocument doc;
        doc.create(filePath.string(), OpenXLSX::XLForceOverwrite);
        OpenXLSX::XLWorkbook wb = doc.workbook();
        std::unordered_map<std::string, unsigned int> duplicateSheets;

        const auto& compounds = table.getCompounds();
        for (const Compound& compound: compounds) {
            // Create worksheet and check if name is a duplicate
            // Excel does not like duplicate sheet names
            std::string sheetName = compound.getName();
            if (wb.worksheetExists(sheetName)) {
                duplicateSheets[sheetName]++;
                sheetName += "_" + std::to_string(duplicateSheets[sheetName]);
                std::cerr << "Duplicate sheet name found: " << compound.getName() << ", renaming to: " << sheetName << std::endl;
            }
            wb.addWorksheet(sheetName);
            OpenXLSX::XLWorksheet sheet = wb.worksheet(sheetName);

            // Add header
            formatHeader(sheet);
            // Add values
            exportCompound(compound, sheet);
        }

    }

}
