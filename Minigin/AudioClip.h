#pragma once
#include <string>
#include <memory>

struct Mix_Chunk;

namespace dae
{
    class AudioClip
    {
    public:
        explicit AudioClip(const std::string& path);
        ~AudioClip();

        AudioClip(const AudioClip&) = delete;
        AudioClip& operator=(const AudioClip&) = delete;
        AudioClip(AudioClip&&) = delete;
        AudioClip& operator=(AudioClip&&) = delete;

        void Play(float volume = 1.0f);
        void Stop();
        void Pause();
        void Resume();
        bool IsLoaded() const { return m_IsLoaded; }
        bool Load();

    private:
        std::string m_FilePath;
        Mix_Chunk* m_pChunk;
        int m_Channel;
        bool m_IsLoaded;
    };
}
