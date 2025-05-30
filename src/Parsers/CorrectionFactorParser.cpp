#include "../../include/Spectrotool/Parsers/CorrectionFactorParser.hpp"
#include "../utils.hpp"

namespace Spectrotool {

    void parseWorkSheet(const OpenXLSX::XLWorksheet& sheet, CorrectionFactor& factor, ParserMessages& msgs) {
        const auto range = sheet.range(OpenXLSX::XLCellReference("A2"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const OpenXLSX::XLCell& row: range) {
            CorrectionValue value;
            value.compoundName = getXLValue<std::string>(row.offset(0, 0));
            value.quantifiedWith = getXLValue<std::string>(row.offset(0, 1));
            value.correctionFactor = getXLValue<double>(row.offset(0, 2));
            value.quantityISTD = getXLValue<double>(row.offset(0, 3));
            factor.addValue(std::move(value));
        }
    }

    ParserMessages parseCorrectionFactor(const CorrectionFactorDesc& desc, CorrectionFactor& factor) {
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
                    parseWorkSheet(sheet, factor, msgs);
                } catch (std::exception& e) {
                    msgs.addError("Invalid Parse!", "UNKNOWN", sheetName);
                }
            }
        }
        return msgs;
    }
}
