#include "SDLSoundSystem.h"
#include "AudioClip.h"
#include <SDL_mixer.h>
#include <iostream>
#include <stdexcept>

namespace dae
{
    class SDLSoundSystem::SDLSoundSystemImpl
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

    SDLSoundSystem::SDLSoundSystem()
        : m_pImpl(std::make_unique<SDLSoundSystemImpl>())
    {
    }

    SDLSoundSystem::~SDLSoundSystem() = default;

    void SDLSoundSystem::Play(const sound_id id, const float volume)
    {
        m_pImpl->Play(id, volume);
    }

    void SDLSoundSystem::Stop(const sound_id id)
    {
        m_pImpl->Stop(id);
    }

    void SDLSoundSystem::Pause(const sound_id id)
    {
        m_pImpl->Pause(id);
    }

    void SDLSoundSystem::Resume(const sound_id id)
    {
        m_pImpl->Resume(id);
    }

    void SDLSoundSystem::StopAll()
    {
        m_pImpl->StopAll();
    }

    void SDLSoundSystem::LoadSound(const sound_id id, const std::string& path)
    {
        m_pImpl->LoadSound(id, path);
    }
}
