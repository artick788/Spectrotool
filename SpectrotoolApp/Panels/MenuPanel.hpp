#pragma once

#include "Panel.hpp"

namespace Spectrotool {

    class MenuPanel: public Panel {
    public:
        MenuPanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

        ~MenuPanel() override = default;

        void render(WindowSize& size) override;

    private:
        void renderOpenMenuItem() const;

        void renderSaveMenuItem() const;

        void renderExportExcelMenuItem() const;

        void renderClearMenuItem();

        void renderClearConfirmation();

    private:
        bool m_ShowClearConfirmation = false;
    };

}