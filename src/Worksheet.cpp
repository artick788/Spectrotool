#include "Worksheet.hpp"

#include <filesystem>

namespace Spectrotool{

    Worksheet::Worksheet(const fs::path& path, const WorksheetDesc& desc){
        if (!fs::exists(path)){
            throw std::runtime_error("File does not exist: " + path.string());
        }

        OpenXLSX::XLDocument doc;
        doc.open(path.string());
        loadWorkSheet(doc.workbook().worksheet(desc.polarSheetName), m_PolarCompounds);
        loadWorkSheet(doc.workbook().worksheet(desc.neutralSheetName), m_NeutralCompounds);
        loadWorkSheet(doc.workbook().worksheet(desc.apolarSheetName), m_ApolarCompounds);

    }

    void Worksheet::loadWorkSheet(const OpenXLSX::XLWorksheet& sheet, std::vector<Compound>& compounds){
        Compound* currentCompound = nullptr;
        uint32_t nextCompound = 0;
        static std::string compoundName = "Compound ";

        // Specify a quadratic region that is 1 column wide and as long as the sheet
        const auto range = sheet.range(OpenXLSX::XLCellReference("A1"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const auto& row: range) {
            std::string value = row.value();
            std::string nextCompoundField = compoundName + std::to_string(nextCompound);
            // Create a new compound and format the name
            if (value.find(nextCompoundField) != std::string::npos) {
                compounds.emplace_back(formatCompoundName(row.value().getString()));
                currentCompound = &compounds.back();
                nextCompound++;
            }
        }
    }

    std::string Worksheet::formatCompoundName(const std::string &name) {
        size_t pos = name.find(':');
        if (pos == std::string::npos) {
            throw std::runtime_error("Invalid compound name: " + name);
        }
        std::string extracted = name.substr(pos + 1);
        return extracted;
    }

}
