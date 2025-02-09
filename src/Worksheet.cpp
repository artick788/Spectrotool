#include "../include/Spectrotool/Worksheet.hpp"

#include <filesystem>

namespace Spectrotool{

    Worksheet::Worksheet(const fs::path& path, const WorksheetDesc& desc){
        if (!fs::exists(path)){
            throw std::runtime_error("File does not exist: " + path.string());
        }

        OpenXLSX::XLDocument doc;
        doc.open(path.string());
        // TODO: Do we really need the distinction between polar, neutral and apolar compounds?
        //      Can we add them to the same vector? Or maybe store the type directly in the Compound object?
        //
        //      ==> No, just store them in a single vector
        loadWorkSheet(doc.workbook().worksheet(desc.polarSheetName), m_PolarCompounds);
        loadWorkSheet(doc.workbook().worksheet(desc.neutralSheetName), m_NeutralCompounds);
        loadWorkSheet(doc.workbook().worksheet(desc.apolarSheetName), m_ApolarCompounds);
    }

    bool Worksheet::hasCompound(const std::string &name, const ST_COMPOUND_TYPE type) const {
        switch (type) {
            case ST_COMPOUND_TYPE_POLAR:    return searchInCompound(name, m_PolarCompounds);
            case ST_COMPOUND_TYPE_NEUTRAL:  return searchInCompound(name, m_NeutralCompounds);
            case ST_COMPOUND_TYPE_APOLAR:   return searchInCompound(name, m_ApolarCompounds);
            default:                        return false;
        }
    }

    std::vector<Compound*> Worksheet::getCompound(const std::string &name, const ST_COMPOUND_TYPE type) {
        switch (type) {
            case ST_COMPOUND_TYPE_POLAR:    return getCompound(name, m_PolarCompounds);
            case ST_COMPOUND_TYPE_NEUTRAL:  return getCompound(name, m_NeutralCompounds);
            case ST_COMPOUND_TYPE_APOLAR:   return getCompound(name, m_ApolarCompounds);
            default:                        return {};
        }
    }


    void Worksheet::loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, std::vector<Compound>& compounds){
        Compound* currentCompound = nullptr;
        uint32_t nextCompound = 1;
        static std::string compoundName = "Compound ";

        // Specify a quadratic region that is 1 column wide and as long as the sheet
        const auto range = sheet.range(OpenXLSX::XLCellReference("A1"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        uint64_t rowNumber = 1;
        for (const auto& row: range) {
            // If the cell is a string, check if it contains the compound name
            if (row.value().type() == OpenXLSX::XLValueType::String) {
                std::string value = row.value();
                std::string nextCompoundField = compoundName + std::to_string(nextCompound);
                // Create a new compound and format the name
                if (value.find(nextCompoundField) != std::string::npos) {
                    compounds.emplace_back(formatCompoundName(value));
                    currentCompound = &compounds.back();
                    nextCompound++;
                }
            }
            // If the cell contains an int, then we know that this row contains a compound entry and we should add if to the current compound
            if (row.value().type() == OpenXLSX::XLValueType::Integer) {
                if (currentCompound == nullptr) {
                    continue;
                }
                addCompound(*currentCompound, row);
            }
            rowNumber++;
        }
    }

    std::string Worksheet::formatCompoundName(const std::string &name) {
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

    void Worksheet::addCompound(Compound &compound, const OpenXLSX::XLCell &row) {
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

    bool Worksheet::searchInCompound(const std::string &name, const std::vector<Compound> &compounds) {
        for (const auto& compound: compounds) {
            if (compound.getName() == name) {
                return true;
            }
        }
        return false;
    }

    std::vector<Compound*> Worksheet::getCompound(const std::string &name, std::vector<Compound> &compounds) {
        std::vector<Compound*> result;
        for (auto& compound: compounds) {
            if (compound.getName() == name) {
                result.push_back(&compound);
            }
        }
        return result;
    }


}
