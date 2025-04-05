#include "SDL_SoundSystem.h"
#include "AudioClip.h"
#include <SDL_mixer.h>
#include <iostream>
#include <stdexcept>

namespace dae
{
    class SDL_SoundSystem::SDLSoundSystemImpl
    {
    public:
        SDLSoundSystemImpl()
        {
            // Initialize SDL_mixer
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                throw std::runtime_error(std::string("SDL_mixer could not initialize! SDL_mixer Error: ") + Mix_GetError());
            }
            
            // Initialize audio channels
            Mix_AllocateChannels(16); // Allocate 16 mixing channels
        }

        ~SDLSoundSystemImpl()
        {
            // Clean up all audio clips
            for (auto& pair : m_AudioClips)
            {
                pair.second.reset();
            }
            m_AudioClips.clear();

            // Close SDL_mixer
            Mix_CloseAudio();
            Mix_Quit();
        }

        void Play(const sound_id id, const float volume)
        {
            auto it = m_AudioClips.find(id);
            if (it != m_AudioClips.end())
            {
                it->second->Play(volume);
            }
        }

        void Stop(const sound_id id)
        {
            auto it = m_AudioClips.find(id);
            if (it != m_AudioClips.end())
            {
                it->second->Stop();
            }
        }

        void Pause(const sound_id id)
        {
            auto it = m_AudioClips.find(id);
            if (it != m_AudioClips.end())
            {
                it->second->Pause();
            }
        }

        void Resume(const sound_id id)
        {
            auto it = m_AudioClips.find(id);
            if (it != m_AudioClips.end())
            {
                it->second->Resume();
            }
        }

        void StopAll()
        {
            Mix_HaltChannel(-1); // Stop all channels
        }

        void LoadSound(const sound_id id, const std::string& path)
        {
            // Check if already loaded
            auto it = m_AudioClips.find(id);
            if (it != m_AudioClips.end())
            {
                return; // Already loaded
            }

            // Create and load new audio clip
            auto audioClip = std::make_unique<AudioClip>(path);
            m_AudioClips[id] = std::move(audioClip);
        }

    private:
        std::unordered_map<sound_id, std::unique_ptr<AudioClip>> m_AudioClips;
    };

    SDL_SoundSystem::SDL_SoundSystem()
        : m_pImpl(std::make_unique<SDLSoundSystemImpl>())
    {
    }

    SDL_SoundSystem::~SDL_SoundSystem() = default;

    void SDL_SoundSystem::Play(const sound_id id, const float volume)
    {
        m_pImpl->Play(id, volume);
    }

    void SDL_SoundSystem::Stop(const sound_id id)
    {
        m_pImpl->Stop(id);
    }

    void SDL_SoundSystem::Pause(const sound_id id)
    {
        m_pImpl->Pause(id);
    }

    void SDL_SoundSystem::Resume(const sound_id id)
    {
        m_pImpl->Resume(id);
    }

    void SDL_SoundSystem::StopAll()
    {
        m_pImpl->StopAll();
    }

    void SDL_SoundSystem::LoadSound(const sound_id id, const std::string& path)
    {
        m_pImpl->LoadSound(id, path);
    }
}
