#include "../include/Spectrotool/SampleListFile.hpp"

namespace Spectrotool {

    SampleListFile::SampleListFile(const SampleListFileDesc &desc) {
        if (!fs::exists(desc.filePath)){
            throw std::runtime_error("File does not exist: " + desc.filePath.string());
        }

        OpenXLSX::XLDocument doc;
        doc.open(desc.filePath.string());
        for (const auto& sheet: doc.workbook().sheetNames()) {
            OpenXLSX::XLWorksheet worksheet = doc.workbook().worksheet(sheet);
            loadSheet(worksheet);
        }
    }

    void SampleListFile::loadSheet(const OpenXLSX::XLWorksheet &sheet) {
        // Specify the range of cells to read, The header is in the first row so skip it
        const auto range = sheet.range(OpenXLSX::XLCellReference("A2"), OpenXLSX::XLCellReference(sheet.rowCount(), 1));
        for (const auto& row: range) {
            SampleInfo sample;
            sample.id = row.offset(0, 0).value().getString();
            sample.weight = std::stod(row.offset(0, 1).value().getString());
            sample.matrix = row.offset(0, 2).value().getString();
            m_Samples[sample.id] = sample;
        }
    }


}