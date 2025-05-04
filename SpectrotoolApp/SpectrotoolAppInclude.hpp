#pragma once

#include <SyriusCore/SyriusCore.hpp>
#include <Spectrotool/MassSpecFile.hpp>
#include <Spectrotool/SampleListFile.hpp>
#include <Spectrotool/CorrectionFactor.hpp>
#include <functional>

namespace Spectrotool {

    using namespace Syrius;

    static float s_MENUBAR_HEIGHT = 18.0f;

    using ErrorHandle = std::function<void(const std::string&)>;

}