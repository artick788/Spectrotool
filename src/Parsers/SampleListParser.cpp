#include "../../include/Spectrotool/Parsers/SampleListParser.hpp"
#include "../utils.hpp"

namespace Spectrotool {

    void parseWorkSheet(const OpenXLSX::XLWorksheet& sheet, const SampleListDesc& desc, SampleList& list, ParserMessages& msgs) {
        const auto range = sheet.range(OpenXLSX::XLCellReference("A2"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const auto& row: range) {
            SampleInfo sample;
            sample.id = getXLValue<std::string>(row.offset(0, 0));
            sample.weight = getXLValue<double>(row.offset(0, 1));
            sample.matrix = getXLValue<std::string>(row.offset(0, 2));
            list[sample.id] = sample;
        }
    }

    ParserMessages parseSampleList(const SampleListDesc& desc, SampleList& list) {
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
                    parseWorkSheet(sheet, desc, list, msgs);
                } catch (std::exception& e) {
                    msgs.addError("Invalid Parse!", "UNKNOWN", sheetName);
                }
            }
        }

        return msgs;
    }

}
