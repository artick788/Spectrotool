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

}
