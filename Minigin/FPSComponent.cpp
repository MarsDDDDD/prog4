// ===== FPSComponent.cpp =====
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

#include <iomanip> // Required for std::setprecision and std::fixed
#include <sstream> // Required for std::stringstream
dae::FPSComponent::FPSComponent() :
    m_fps(0.0f),
    m_timeAccumulator(0.0f),
    m_frameCount(0)
{
    m_frameTimes.reserve(m_maxSamples); // Reserve space to avoid reallocations
}

void dae::FPSComponent::Update(float deltaTime)
{
    m_timeAccumulator += deltaTime;
    m_frameCount++;

    // Add the new frame time to the buffer
    m_frameTimes.push_back(deltaTime);

    // If we have more samples than the maximum, remove the oldest one
    if (m_frameTimes.size() > m_maxSamples)
    {
        m_frameTimes.erase(m_frameTimes.begin()); // Remove from front (FIFO)
    }

    if (m_timeAccumulator >= 0.5f) // Recalculate a few times per second.  Not every frame.
    {
        float totalTime = 0.0f;
        for (float frameTime : m_frameTimes)
        {
            totalTime += frameTime;
        }
        m_fps = (m_frameTimes.size() > 0) ? (1.0f / (totalTime / static_cast<float>(m_frameTimes.size()))) : 0.0f;

        //Update the text component

        auto textComponent = m_gameObject->GetComponent<TextComponent>();
        if (textComponent)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1) << m_fps; // Format to 1 decimal place
            textComponent->SetText(stream.str() + " FPS");
        }        m_timeAccumulator -= 0.5f; // reset the accumulator, but don't zero it.
        m_frameCount = 0;          //Reset frame count for the next measurement interval
    }
}

float dae::FPSComponent::GetFPS() const
{
    return m_fps;
}