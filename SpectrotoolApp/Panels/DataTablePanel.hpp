#pragma once

#include "Panel.hpp"

namespace Spectrotool{

        class DataTablePanel: public Panel{
        public:
            DataTablePanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

            ~DataTablePanel() override = default;

            void render(WindowSize &size) override;

        private:

            void renderTable(const UP<DataTable>& project);

            static void renderCompound(const Compound& compound);

        };

}