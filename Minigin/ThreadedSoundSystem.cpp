#include "ThreadedSoundSystem.h"

namespace dae
{
    ThreadedSoundSystem::ThreadedSoundSystem(std::unique_ptr<SoundSystem>&& ss)
        : m_pRealSS(std::move(ss))
        , m_WorkerThread(&ThreadedSoundSystem::WorkerFunction, this)
    {
    }

    ThreadedSoundSystem::~ThreadedSoundSystem()
    {
        m_EventQueue.Stop();
        if (m_WorkerThread.joinable())
        {
            m_WorkerThread.join();
        }
    }

    void ThreadedSoundSystem::WorkerFunction()
    {
        while (auto event = m_EventQueue.Pop())
        {
            event();
        }
    }

    void ThreadedSoundSystem::Play(const sound_id id, const float volume)
    {
        m_EventQueue.Push([this, id, volume] {
            m_pRealSS->Play(id, volume);
        });
    }

    void ThreadedSoundSystem::Stop(const sound_id id)
    {
        m_EventQueue.Push([this, id] {
            m_pRealSS->Stop(id);
        });
    }

    void ThreadedSoundSystem::Pause(const sound_id id)
    {
        m_EventQueue.Push([this, id] {
            m_pRealSS->Pause(id);
        });
    }

    void ThreadedSoundSystem::Resume(const sound_id id)
    {
        m_EventQueue.Push([this, id] {
            m_pRealSS->Resume(id);
        });
    }

    void ThreadedSoundSystem::StopAll()
    {
        m_EventQueue.Push([this] {
            m_pRealSS->StopAll();
        });
    }

    void ThreadedSoundSystem::LoadSound(const sound_id id, const std::string& path)
    {
        m_EventQueue.Push([this, id, path] {
            m_pRealSS->LoadSound(id, path);
        });
    }
}
