#pragma once

#include "ParserMessages.hpp"
#include "../CorrectionFactor.hpp"

namespace Spectrotool {

    struct CorrectionFactorDesc {
        fs::path filePath;
    };

    ParserMessages parseCorrectionFactor(const CorrectionFactorDesc& desc, CorrectionFactor& factor);

}
