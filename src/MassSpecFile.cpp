#include "../include/Spectrotool/MassSpecFile.hpp"

#include <filesystem>

#include "utils.hpp"

namespace Spectrotool{

    void addCompound1(Compound &compound, const OpenXLSX::XLCell &row) {
        CompoundValue value;
        value.name = getXLValue<std::string>(row.offset(0, 2));
        value.id = getXLValue<std::string>(row.offset(0, 3));
        value.rt = getXLValue<double>(row.offset(0, 4));
        value.area = getXLValue<double>(row.offset(0, 5));
        value.isArea = getXLValue<double>(row.offset(0, 6));
        value.sDivN = getXLValue<double>(row.offset(0, 7));

        compound.addValue(std::move(value));
    }

    MassSpecFile::MassSpecFile(const MassSpecFileDesc& desc){
        if (!fs::exists(desc.filePath)){
            throw std::runtime_error("File does not exist: " + desc.filePath.string());
        }

        OpenXLSX::XLDocument doc;
        doc.open(desc.filePath.string());
        for (const auto& sheetName: desc.sheetNames) {
            if (!doc.workbook().worksheetExists(sheetName)) {
                throw std::runtime_error("Sheet: " + sheetName + " does not exist in file: " + desc.filePath.string());
            }
            loadWorkSheet(doc.workbook().worksheet(sheetName), desc);
        }
    }

    bool MassSpecFile::hasCompound(const std::string &name) const {
        for (const auto& compound: m_Compounds) {
            if (compound.getName() == name) {
                return true;
            }
        }
        return false;
    }

    std::vector<Compound*> MassSpecFile::getCompound(const std::string &name) {
        std::vector<Compound*> result;
        for (auto& compound: m_Compounds) {
            if (compound.getName() == name) {
                result.push_back(&compound);
            }
        }
        return result;
    }

    void MassSpecFile::setSampleInfo(const SampleListFile &sampleListFile) {
        for (auto& compound: m_Compounds) {
            compound.setSampleInfo(sampleListFile);
        }
    }

    void MassSpecFile::correct(const CorrectionFactor &factor) {
        for (Compound& compound: m_Compounds) {
            if (!factor.hasCorrectionValue(compound.getName())) {
                // Invalid Input!
                throw std::runtime_error("Compound: " + compound.getName() + " does not have a correction factor!");
            }
            const CorrectionValue& correction = factor.getCorrectionValue(compound.getName());
            compound.correct(correction);
        }
    }

    nlohmann::json MassSpecFile::toJson() const {
        nlohmann::json json;
        for (const auto& compound: m_Compounds) {
            json[compound.getName()] = compound.toJson();
        }
        return json;
    }

    void MassSpecFile::fromJson(const nlohmann::json &json) {
        for (const auto& [name, compoundJson]: json.items()) {
            m_Compounds.emplace_back(name);
            m_Compounds.back().fromJson(compoundJson);
            m_ReadCompoundCount++;
            m_SampleCount += m_Compounds.back().getValues().size();
        }
    }

    void MassSpecFile::exportToExcel(const MassSpecFileExportDesc &config) const {
        OpenXLSX::XLDocument doc;
        std::cout << "Exporting to: " << config.filePath << std::endl;
        doc.create(config.filePath.string(), OpenXLSX::XLForceOverwrite);
        OpenXLSX::XLWorkbook wb = doc.workbook();
        std::unordered_map<std::string, unsigned int> duplicateSheets;
        for (const auto& compound: m_Compounds) {
            std::string sheetName = compound.getName();
            sanitizeString(sheetName);
            if (wb.worksheetExists(sheetName)) {
                duplicateSheets[sheetName]++;
                sheetName += "_" + std::to_string(duplicateSheets[sheetName]);
                std::cerr << "Duplicate sheet name found: " << compound.getName() << ", renaming to: " << sheetName << std::endl;
            }

            wb.addWorksheet(sheetName);
            OpenXLSX::XLWorksheet sheet = wb.worksheet(sheetName);
            formatHeader(sheet);

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
        doc.workbook().deleteSheet("Sheet1"); // Remove the default sheet
        doc.save();
    }

    void MassSpecFile::loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, const MassSpecFileDesc& desc){
        Compound* currentCompound = nullptr;
        static std::string compoundName = "Compound ";

        // Specify a quadratic region that is 1 column wide and as long as the sheet
        const auto range = sheet.range(OpenXLSX::XLCellReference("A4"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const auto& row: range) {
            // If the cell is a string, check if it contains the compound name
            if (row.value().type() == OpenXLSX::XLValueType::String) {
                // Create a new compound and format the name
                std::string value = row.value();
                if (value.find(compoundName) != std::string::npos) {
                    if (m_CompoundNames.find(value) != m_CompoundNames.end()) {
                        throw std::runtime_error("Duplicate compound name: " + value);
                    }
                    m_ReadCompoundCount++;
                    if (!desc.excludeFilter.empty() && filterCompound(value, desc.excludeFilter)) {
                        currentCompound = nullptr; // Skip all subsequent rows until the next compound is found
                        continue;
                    }
                    m_Compounds.emplace_back(formatCompoundName(value));
                    currentCompound = &m_Compounds.back();
                }
            }
            // If the cell contains an int, then we know that this row contains a compound entry and we should add if to the current compound
            if (row.value().type() == OpenXLSX::XLValueType::Integer) {
                if (currentCompound == nullptr) {
                    continue;
                }
                addCompound1(*currentCompound, row);
                m_SampleCount++;
            }
        }
    }

    bool MassSpecFile::filterCompound(const std::string &name, const std::string &filter) {
        return name.find(filter) != std::string::npos;
    }

    std::string MassSpecFile::formatCompoundName(const std::string &name) {
        // Compound names are formatted as "Compound X: <name>", remove the "Compound X: " part
        const size_t pos = name.find(':');
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid compound name: " + name);
        }
        std::string extracted = name.substr(pos + 1);
        if (extracted.empty()) {
            throw std::runtime_error("Invalid compound name: " + name);
        }
        // Remove leading spaces
        if (std::size_t start_str = extracted.find_first_not_of(' '); start_str != std::string::npos) {
            extracted.erase(0, start_str);
        }
        // finally sanitize the string
        sanitizeString(extracted);
        return extracted;
    }
    
    void MassSpecFile::formatHeader(const OpenXLSX::XLWorksheet &sheet){
        sheet.cell("A1").value() = "Name";
        sheet.cell("B1").value() = "ID";
        sheet.cell("C1").value() = "RT";
        sheet.cell("D1").value() = "Area";
        sheet.cell("E1").value() = "IS Area";
        sheet.cell("F1").value() = "S/N";
        sheet.cell("G1").value() = "Weight";
        sheet.cell("H1").value() = "Matrix";
    }

}
