#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>

namespace dae
{
    class LoggingSoundSystem final : public SoundSystem
    {
    public:
        explicit LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss);
        
        void Play(const sound_id id, const float volume) override;
        void Stop(const sound_id id) override;
        void Pause(const sound_id id) override;
        void Resume(const sound_id id) override;
        void StopAll() override;
        void LoadSound(const sound_id id, const std::string& path) override;
        
    private:
        std::unique_ptr<SoundSystem> m_pRealSS;
    };
}
