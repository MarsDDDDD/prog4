#include "SoundEventQueue.h"

namespace dae
{
    SoundEventQueue::~SoundEventQueue()
    {
        Stop();
    }

    void SoundEventQueue::Push(std::function<void()> event)
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Events.push(std::move(event));
        }
        m_CondVar.notify_one();
    }

    std::function<void()> SoundEventQueue::Pop()
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_CondVar.wait(lock, [this] { return !m_Events.empty() || !m_Running; });
        
        if (!m_Running && m_Events.empty())
            return nullptr;
            
        auto event = std::move(m_Events.front());
        m_Events.pop();
        return event;
    }

    void SoundEventQueue::Stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Running = false;
        }
        m_CondVar.notify_all();
    }
}
