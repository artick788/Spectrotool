#include "SpectrotoolApp.hpp"

namespace Spectrotool {

    SpectrotoolApp::SpectrotoolApp() {
        WindowDesc wDesc;
        wDesc.width = 1280;
        wDesc.height = 720;
        wDesc.title = "Spectrotool";
        m_Window = createWindow(wDesc);

        ContextDesc cDesc;
        cDesc.api = SR_API_D3D11;
        m_Context = m_Window->createContext(cDesc);
        m_Context->setVerticalSynchronisation(true);
        m_Context->setClearColor(1.0f, 0.0, 0.0, 1.0f);
        m_Window->createImGuiContext();

        m_Store = createUP<Store>();
        m_MenuPanel = createUP<MenuPanel>(m_Window, m_Context, m_Store);
        m_FilePanel = createUP<FilePanel>(m_Window, m_Context, m_Store);
    }

    void SpectrotoolApp::run() {
        while (m_Window->isOpen()) {
            m_Window->pollEvents();
            if (m_Window->hasEvent()) {
                auto event = m_Window->getNextEvent();
                processEvent(event);
            }

            m_Context->clear();
            m_Context->beginRenderPass();
            renderImGui();
            m_Context->endRenderPass();
            m_Context->swapBuffers();
        }
    }

    void SpectrotoolApp::processEvent(const Event &event) const {
        switch (event.type) {
            case SR_EVENT_WINDOW_CLOSED: {
                m_Window->close();
                break;
            }
            default: break;
        }
    }

    void SpectrotoolApp::renderImGui() {
        m_Window->onImGuiBegin();

        WindowSize size;
        m_MenuPanel->render(size);
        size.y += s_MENUBAR_HEIGHT;
        m_FilePanel->render(size);

        m_Store->renderErrorMsg();
        m_Window->onImGuiEnd();
    }

}
