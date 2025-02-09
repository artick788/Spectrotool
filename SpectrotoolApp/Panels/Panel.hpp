#pragma once

#include "../SpectrotoolAppInclude.hpp"
#include "../Store.hpp"

namespace Spectrotool {

    struct WindowSize {
        float width = 0.0f;
        float height = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
    };

    class Panel {
    public:
        Panel(UP<SyriusWindow>& window, ResourceView<Context>& context, UP<Store>& store);

        virtual ~Panel() = default;

        virtual void render(WindowSize& size) = 0;

    protected:
        UP<SyriusWindow>& m_Window;
        ResourceView<Context>& m_Context;
        UP<Store>& m_Store;
    };



}
