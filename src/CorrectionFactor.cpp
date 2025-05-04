#include "../include/Spectrotool/CorrectionFactor.hpp"

#include "utils.hpp"

namespace Spectrotool{

    CorrectionFactor::CorrectionFactor(const fs::path& filePath){
        if (!fs::exists(filePath)) {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }
        OpenXLSX::XLDocument doc;
        doc.open(filePath.string());
        const std::string firstSheetName = doc.workbook().sheetNames().front();
        const OpenXLSX::XLWorksheet& sheet = doc.workbook().worksheet(firstSheetName);
        loadWorkSheet(sheet);
    }

    const CorrectionValue &CorrectionFactor::getCorrectionValue(const std::string &name) const {
        return m_CorrectionValues.at(name);
    }

    void CorrectionFactor::loadWorkSheet(const OpenXLSX::XLWorksheet &sheet) {
        // Same trick to iterate row wise
        const auto range = sheet.range(OpenXLSX::XLCellReference("A2"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const OpenXLSX::XLCell& row: range) {
            CorrectionValue value;
            value.compoundName = getXLValue<std::string>(row.offset(0, 0));
            value.quantifiedWith = getXLValue<std::string>(row.offset(0, 1));
            value.correctionFactor = getXLValue<double>(row.offset(0, 2));
            value.quantityISTD = getXLValue<double>(row.offset(0, 3));
            m_CorrectionValues.emplace(value.compoundName, value);
        }
    }

}
