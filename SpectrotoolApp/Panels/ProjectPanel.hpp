#pragma once

#include "Panel.hpp"

namespace Spectrotool{

        class ProjectPanel: public Panel{
        public:
            ProjectPanel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

            ~ProjectPanel() override = default;

            void render(WindowSize &size) override;

        private:

            void renderProject(const UP<Project>& project);

            static void renderCompound(const Compound& compound);

        };

}