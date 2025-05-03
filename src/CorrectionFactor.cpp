#include "../include/Spectrotool/CorrectionFactor.hpp"

namespace Spectrotool{

    CorrectionFactor::CorrectionFactor(const fs::path& filePath){
        if (!fs::exists(filePath)) {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }
        OpenXLSX::XLDocument doc;
        doc.open(filePath.string());
    }
}