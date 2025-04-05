#include "AudioClip.h"
#include <SDL_mixer.h>
#include <iostream>

namespace dae
{
    AudioClip::AudioClip(const std::string& path)
        : m_FilePath(path)
        , m_pChunk(nullptr)
        , m_Channel(-1)
        , m_IsLoaded(false)
    {
        Load();
    }

    AudioClip::~AudioClip()
    {
        if (m_pChunk)
        {
            Mix_FreeChunk(m_pChunk);
            m_pChunk = nullptr;
        }
    }

    bool AudioClip::Load()
    {
        if (m_IsLoaded)
            return true;

        m_pChunk = Mix_LoadWAV(m_FilePath.c_str());
        if (!m_pChunk)
        {
            std::cerr << "Failed to load audio clip: " << m_FilePath << ". SDL_mixer Error: " << Mix_GetError() << std::endl;
            return false;
        }

        m_IsLoaded = true;
        return true;
    }

    void AudioClip::Play(float volume)
    {
        if (!m_IsLoaded && !Load())
            return;

        // Set volume (0-128)
        Mix_VolumeChunk(m_pChunk, static_cast<int>(volume * 128.0f));
        
        // Play on first available channel, no looping
        m_Channel = Mix_PlayChannel(-1, m_pChunk, 0);
        
        if (m_Channel == -1)
        {
            std::cerr << "Failed to play audio clip. SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    void AudioClip::Stop()
    {
        if (m_Channel != -1)
        {
            Mix_HaltChannel(m_Channel);
            m_Channel = -1;
        }
    }

    void AudioClip::Pause()
    {
        if (m_Channel != -1)
        {
            Mix_Pause(m_Channel);
        }
    }

    void AudioClip::Resume()
    {
        if (m_Channel != -1)
        {
            Mix_Resume(m_Channel);
        }
    }
}
