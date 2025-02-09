#include "Panel.hpp"

namespace Spectrotool {

    Panel::Panel(UP<SyriusWindow> &window, ResourceView<Context> &context, UP<Store> &store):
    m_Window(window),
    m_Context(context),
    m_Store(store) {

    }


}
