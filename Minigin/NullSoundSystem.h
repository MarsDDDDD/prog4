#pragma once
#include "SoundSystem.h"

namespace dae
{
    class NullSoundSystem final : public SoundSystem
    {
    public:
        void Play(const sound_id, const float) override {};
        void Stop(const sound_id) override {};
        void Pause(const sound_id) override {};
        void Resume(const sound_id) override {};
        void StopAll() override {};
        void LoadSound(const sound_id, const std::string&) override {};
    };
}
