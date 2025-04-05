#pragma once
#include "SoundSystem.h"
#include "SoundEventQueue.h"
#include <memory>
#include <thread>
#include <string>

namespace dae
{
    class ThreadedSoundSystem final : public SoundSystem
    {
    public:
        explicit ThreadedSoundSystem(std::unique_ptr<SoundSystem>&& ss);
        ~ThreadedSoundSystem() override;
        
        // Disable copy/move
        ThreadedSoundSystem(const ThreadedSoundSystem&) = delete;
        ThreadedSoundSystem& operator=(const ThreadedSoundSystem&) = delete;
        ThreadedSoundSystem(ThreadedSoundSystem&&) = delete;
        ThreadedSoundSystem& operator=(ThreadedSoundSystem&&) = delete;
        
        void Play(const sound_id id, const float volume) override;
        void Stop(const sound_id id) override;
        void Pause(const sound_id id) override;
        void Resume(const sound_id id) override;
        void StopAll() override;
        void LoadSound(const sound_id id, const std::string& path) override;
        
    private:
        std::unique_ptr<SoundSystem> m_pRealSS;
        SoundEventQueue m_EventQueue;
        std::thread m_WorkerThread;
        
        void WorkerFunction();
    };
}
