#pragma once

#include "SpectrotoolAppInclude.hpp"

#include <Spectrotool/Worksheet.hpp>

namespace Spectrotool {

    class SpectrotoolApp {
    public:
        SpectrotoolApp();

        ~SpectrotoolApp() = default;

        void run();

    private:

        void processEvent(const Event& event) const;

        void renderImGui();

    private:
        UP<SyriusWindow> m_Window;
        ResourceView<Context> m_Context;
    };

}

