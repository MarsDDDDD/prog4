#include "LoggingSoundSystem.h"
#include <iostream>

namespace dae
{
    LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss)
        : m_pRealSS(std::move(ss))
    {
    }

    void LoggingSoundSystem::Play(const sound_id id, const float volume)
    {
        std::cout << "Sound: Playing sound " << id << " at volume " << volume << std::endl;
        m_pRealSS->Play(id, volume);
    }

    void LoggingSoundSystem::Stop(const sound_id id)
    {
        std::cout << "Sound: Stopping sound " << id << std::endl;
        m_pRealSS->Stop(id);
    }

    void LoggingSoundSystem::Pause(const sound_id id)
    {
        std::cout << "Sound: Pausing sound " << id << std::endl;
        m_pRealSS->Pause(id);
    }

    void LoggingSoundSystem::Resume(const sound_id id)
    {
        std::cout << "Sound: Resuming sound " << id << std::endl;
        m_pRealSS->Resume(id);
    }

    void LoggingSoundSystem::StopAll()
    {
        std::cout << "Sound: Stopping all sounds" << std::endl;
        m_pRealSS->StopAll();
    }

    void LoggingSoundSystem::LoadSound(const sound_id id, const std::string& path)
    {
        std::cout << "Sound: Loading sound " << id << " from " << path << std::endl;
        m_pRealSS->LoadSound(id, path);
    }
}
