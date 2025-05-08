#pragma once

#include "SpectrotoolAppInclude.hpp"
#include "Store.hpp"
#include "Panels/OverviewPanel.hpp"
#include "Panels/MenuPanel.hpp"
#include "Panels/DataTablePanel.hpp"

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
        UP<Store> m_Store;

        UP<MenuPanel> m_MenuPanel;
        UP<OverviewPanel> m_FilePanel;
        UP<DataTablePanel> m_ProjectPanel;
    };

}

