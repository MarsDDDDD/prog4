#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

#include <iomanip>   // For std::setprecision, std::fixed
#include <sstream>   

namespace dae
{
    FPSComponent::FPSComponent(GameObject* pOwner)
        : BaseComponent(pOwner)
        , m_fps(0.0f)
        , m_timeAccumulator(0.0f)
        , m_frameCount(0)
    {
        m_frameTimes.reserve(m_maxSamples);
    }

    void FPSComponent::Update(float deltaTime)
    {
        m_timeAccumulator += deltaTime;
        m_frameCount++;

        // Add the new frame time
        m_frameTimes.emplace_back(deltaTime);
        if (m_frameTimes.size() > m_maxSamples)
        {
            m_frameTimes.erase(m_frameTimes.begin());
        }

        // Recompute every 0.5s
        if (m_timeAccumulator >= 0.5f)
        {
            float totalTime = 0.0f;
            for (float frameTime : m_frameTimes)
            {
                totalTime += frameTime;
            }

            // Calculate average
            if (!m_frameTimes.empty())
            {
                float avgFrameTime = totalTime / static_cast<float>(m_frameTimes.size());
                m_fps = 1.0f / avgFrameTime;
            }
            else
            {
                m_fps = 0.0f;
            }

            // Update the text component
            if (m_pTextComponent)
            {
                // We already have a pointer, so update it
                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << m_fps;
                m_pTextComponent->SetText(stream.str() + " FPS");
            }
            else
            {
                // Try retrieving a TextComponent again
                auto textComp = m_pGameObject->GetComponent<TextComponent>();
                if (textComp)
                {
                    m_pTextComponent = textComp;
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(1) << m_fps;
                    m_pTextComponent->SetText(stream.str() + " FPS");
                }
            }

            // Reset for next measurement
            m_timeAccumulator -= 0.5f;
            m_frameCount = 0;
        }
    }

    float FPSComponent::GetFPS() const
    {
        return m_fps;
    }
}
