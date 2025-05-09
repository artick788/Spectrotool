#pragma once

#include "ParserMessages.hpp"
#include "../SampleList.hpp"

namespace Spectrotool {

    struct SampleListDesc {
        fs::path filePath = fs::current_path();
    };

    ParserMessages parseSampleList(const SampleListDesc& desc, SampleList& list);

}