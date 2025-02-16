#include "Worker.hpp"

namespace Spectrotool{

    Worker::Worker(const std::string& name):
    m_IsRunning(true),
    m_Name(name),
    m_Thread([this]{
        threadFunc();
    }){
        if (m_Name.empty()){
            m_Name = "Worker" + std::to_string(reinterpret_cast<size_t>(this));
        }

    }

    Worker::~Worker() {
        if (m_IsRunning){
            stop();
        }
    }

    void Worker::stop() {
        {
            std::lock_guard<std::mutex> lk(m_Mutex);
            if (!m_IsRunning){
                return;
            }
            m_IsRunning = false;
        }

        m_Condition.notify_all();
        m_Thread.join();
    }

    void Worker::threadFunc() {
        try{
            for (;;){
                decltype(m_Queue) queue;
                {
                    std::unique_lock<std::mutex> lk(m_Mutex);
                    m_Condition.wait(lk, [&] {
                        return !m_Queue.empty() + !m_IsRunning;
                    });
                    if (!m_IsRunning){
                        for (auto& func: m_Queue){
                            func();
                        }
                        m_Queue.clear();
                        return;
                    }
                    std::swap(m_Queue, queue);
                }
                for (auto& func: queue){
                    func();
                }
            }
        } catch (std::exception& e){
            std::cerr << "Worker: " + m_Name + " received exception: " << e.what() << std::endl;
            std::abort();
        } catch (...){
            std::cerr << "Worker: " + m_Name + " received unknown exception" << std::endl;
            std::abort();
        }
    }

    const std::string &Worker::getName() const {
        return m_Name;
    }

    bool Worker::isRunning() const {
        return m_IsRunning;
    }

}
