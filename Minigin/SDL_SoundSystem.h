#pragma once
#include "SoundSystem.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace dae
{
    class AudioClip;

    class SDL_SoundSystem final : public SoundSystem
    {
    private:
        class SDLSoundSystemImpl;
        std::unique_ptr<SDLSoundSystemImpl> m_pImpl;

    public:
        SDL_SoundSystem();
        ~SDL_SoundSystem() override;

        void Play(const sound_id id, const float volume) override;
        void Stop(const sound_id id) override;
        void Pause(const sound_id id) override;
        void Resume(const sound_id id) override;
        void StopAll() override;
        void LoadSound(const sound_id id, const std::string& path) override;
    };
}
