#include "SpectrotoolApp.hpp"

namespace Spectrotool {

    SpectrotoolApp::SpectrotoolApp() {
        WindowDesc wDesc;
        wDesc.width = 1280;
        wDesc.height = 720;
        wDesc.title = "Spectrotool";
        m_Window = createWindow(wDesc);

        ContextDesc cDesc;
        cDesc.api = SR_API_OPENGL;
        m_Context = m_Window->createContext(cDesc);
        m_Context->setVerticalSynchronisation(true);
        m_Context->setClearColor(1.0f, 0.0, 0.0, 1.0f);
        m_Window->createImGuiContext();

        m_Store = createUP<Store>();
        m_MenuPanel = createUP<MenuPanel>(m_Window, m_Context, m_Store);
        m_FilePanel = createUP<OverviewPanel>(m_Window, m_Context, m_Store);
        m_ProjectPanel = createUP<DataTablePanel>(m_Window, m_Context, m_Store);
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
            case SR_EVENT_WINDOW_RESIZED: {
                m_Context->onResize(event.windowWidth, event.windowWidth);
                break;
            }
            default: break;
        }
    }

    void SpectrotoolApp::renderImGui() {
        m_Window->onImGuiBegin();

        WindowSize menuSize;
        menuSize.x = 0.0f;
        menuSize.y = 0.0f;
        menuSize.width = m_Window->getWidth();
        menuSize.height = s_MENUBAR_HEIGHT;
        m_MenuPanel->render(menuSize);

        WindowSize leftSize;
        leftSize.x = 0.0f;
        leftSize.y += s_MENUBAR_HEIGHT;
        leftSize.width = 300.0f;
        leftSize.height = m_Window->getHeight() - s_MENUBAR_HEIGHT;
        m_FilePanel->render(leftSize);

        WindowSize rightSize;
        rightSize.x = leftSize.width;
        rightSize.y += s_MENUBAR_HEIGHT;
        rightSize.width = m_Window->getWidth() - leftSize.width;
        rightSize.height = m_Window->getHeight() - s_MENUBAR_HEIGHT;
        m_ProjectPanel->render(rightSize);


        m_Store->renderErrorMsg();
        m_Window->onImGuiEnd();
    }

}
