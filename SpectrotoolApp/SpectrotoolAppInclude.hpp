#pragma once

#include <SyriusCore/SyriusCore.hpp>
#include <Spectrotool/Parsers/DataTableParser.hpp>
#include <Spectrotool/Parsers/SampleListParser.hpp>
#include <Spectrotool/Parsers/CorrectionFactorParser.hpp>
#include <functional>

namespace Spectrotool {

    using namespace Syrius;

    static float s_MENUBAR_HEIGHT = 18.0f;

    using ErrorHandle = std::function<void(const std::string&)>;

}