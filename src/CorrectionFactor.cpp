#include "../include/Spectrotool/CorrectionFactor.hpp"

#include "utils.hpp"

namespace Spectrotool{

    void CorrectionFactor::addValue(CorrectionValue &&value) {
        m_CorrectionValues.emplace(value.compoundName, value);
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
