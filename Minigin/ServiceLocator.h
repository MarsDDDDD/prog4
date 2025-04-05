#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
    class ServiceLocator final
    {
    private:
        static std::unique_ptr<SoundSystem> m_pSoundSystemInstance;
        static SoundSystem* m_pNullSoundSystem;

    public:
        static SoundSystem& GetSoundSystem() { return *m_pSoundSystemInstance; }
        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);
        static void Cleanup();
    };
}
