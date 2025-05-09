#pragma once

#include "Panel.hpp"

namespace Spectrotool {

    class OverviewPanel: public Panel {
    public:
        OverviewPanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

        ~OverviewPanel() override = default;

        void render(WindowSize &size) override;

    private:

        void renderPanelContent(WindowSize &size);

        void renderDataTableFileSelector();

        void renderProjectOverview() const;

    private:
        bool m_OpenDataFileSelector = false;
    };

}