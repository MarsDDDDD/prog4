#pragma once
#include <string>

namespace dae
{
    using sound_id = unsigned short;

    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;
        virtual void Play(const sound_id id, const float volume) = 0;
        virtual void Stop(const sound_id id) = 0;
        virtual void Pause(const sound_id id) = 0;
        virtual void Resume(const sound_id id) = 0;
        virtual void StopAll() = 0;
        virtual void LoadSound(const sound_id id, const std::string& path) = 0;
    };
}
