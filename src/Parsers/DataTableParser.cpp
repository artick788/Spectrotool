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

    void parseWorkSheet(const OpenXLSX::XLWorksheet& sheet, const DataTableDesc& desc, DataTable& table, ParserMessages& msgs) {
        Compound* current = nullptr;
        std::unordered_set<std::string> compoundNames;

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
                    if (compoundNames.find(formattedName) != compoundNames.end()) {
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
        for (const auto& sheetName: document.workbook().sheetNames()) {
            const OpenXLSX::XLWorksheet& sheet = document.workbook().worksheet(sheetName);
            if (sheet.rowCount() == 0 || sheet.columnCount() == 0) {
                msgs.addWarning("Sheet is empty!", "A1", sheetName);
            }
            else {
                try {
                    parseWorkSheet(sheet, desc, table, msgs);
                } catch (std::exception& e) {
                    msgs.addError("Invalid Parse!", "UNKNOWN", sheetName);
                }
            }
        }

        return msgs;
    }

    void exportDataTable(const fs::path& filePath) {

    }

}
