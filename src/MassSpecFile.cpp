#include "../include/Spectrotool/MassSpecFile.hpp"

#include <filesystem>

namespace Spectrotool{

    void addCompound(Compound &compound, const OpenXLSX::XLCell &row) {
        CompoundValue value;
        value.name = row.offset(0, 2).value().getString();
        value.id = row.offset(0, 3).value().getString();
        if (const std::string rt = row.offset(0, 4).value().getString(); !rt.empty()) {
            value.rt = std::stod(rt);
        }
        if (const std::string area = row.offset(0, 5).value().getString(); !area.empty()) {
            value.area = std::stod(area);
        }
        if (const std::string isArea = row.offset(0, 6).value().getString(); !isArea.empty()) {
            value.isArea = std::stod(isArea);
        }
        if (const std::string sDivN = row.offset(0, 7).value().getString(); !sDivN.empty()) {
            value.sDivN = std::stod(sDivN);
        }
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
            static const std::string invalidChars = "\\/:?*[]";
            if (sheetName.find_first_of(invalidChars) != std::string::npos) {
                std::cerr << "Invalid characters found in sheet name: " << sheetName << ", replacing with '_'" << std::endl;
                for (const auto& c: invalidChars) {
                    std::replace(sheetName.begin(), sheetName.end(), c, '_');
                }
            }
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
                sheet.cell("A" + std::to_string(row)).value() = compoundValue.name;
                sheet.cell("B" + std::to_string(row)).value() = compoundValue.id;
                sheet.cell("C" + std::to_string(row)).value() = compoundValue.rt;
                sheet.cell("D" + std::to_string(row)).value() = compoundValue.area;
                sheet.cell("E" + std::to_string(row)).value() = compoundValue.isArea;
                sheet.cell("F" + std::to_string(row)).value() = compoundValue.sDivN;
                sheet.cell("G" + std::to_string(row)).value() = compoundValue.weight;
                sheet.cell("H" + std::to_string(row)).value() = compoundValue.matrix;
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
                if (std::string value = row.value(); value.find(compoundName) != std::string::npos) {
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
                addCompound(*currentCompound, row);
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
