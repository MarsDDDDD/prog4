#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace dae
{
    class SoundEventQueue
    {
    public:
        SoundEventQueue() = default;
        ~SoundEventQueue();
        
        // Disable copy/move
        SoundEventQueue(const SoundEventQueue&) = delete;
        SoundEventQueue& operator=(const SoundEventQueue&) = delete;
        SoundEventQueue(SoundEventQueue&&) = delete;
        SoundEventQueue& operator=(SoundEventQueue&&) = delete;
        
        void Push(std::function<void()> event);
        std::function<void()> Pop();
        void Stop();
        
    private:
        std::queue<std::function<void()>> m_Events;
        std::mutex m_Mutex;
        std::condition_variable m_CondVar;
        bool m_Running{true};
    };
}
