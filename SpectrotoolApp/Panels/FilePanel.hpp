#pragma once

#include "Panel.hpp"

namespace Spectrotool {

    class FilePanel: public Panel {
    public:
        FilePanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

        ~FilePanel() override = default;

        void render(WindowSize &size) override;

    private:

        void renderPanelContent(WindowSize &size);

        void renderFileSelector();

        void renderProjectOverview();

    private:
        bool m_OpenFileSelector = false;

    };

}