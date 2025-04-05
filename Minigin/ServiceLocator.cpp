#include "ServiceLocator.h"
#include "NullSoundSystem.h"

namespace dae
{
    // Initialize static members
    std::unique_ptr<SoundSystem> ServiceLocator::m_pSoundSystemInstance = nullptr;
    SoundSystem* ServiceLocator::m_pNullSoundSystem = nullptr;

    void ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
    {
        // Initialize null sound system on first use if needed
        if (m_pNullSoundSystem == nullptr)
        {
            m_pNullSoundSystem = new NullSoundSystem();
        }

        if (ss == nullptr)
        {
            m_pSoundSystemInstance = std::unique_ptr<SoundSystem>(m_pNullSoundSystem);
        }
        else
        {
            m_pSoundSystemInstance = std::move(ss);
        }
    }

    void ServiceLocator::Cleanup()
    {
        m_pSoundSystemInstance.reset();
        delete m_pNullSoundSystem;
        m_pNullSoundSystem = nullptr;
    }
}
