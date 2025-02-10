#include "FPSComponent.h"
#include "Font.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <iomanip> // For std::setprecision
#include <sstream>  // For std::stringstream


namespace dae {

    FPSComponent::FPSComponent(std::shared_ptr<Font> font)
        : m_deltaTimeSum(0.0f)
        , m_frameCount(0)
        , m_fps(0.0f)
        , m_textComponent(std::make_shared<TextComponent>("FPS: 0", font)) // Initialize TextComponent
        , m_lastTime(std::chrono::high_resolution_clock::now())
    {
        // Add TextComponent to the same GameObject as FPSComponent
        if (GetGameObject())
        {
            GetGameObject()->AddComponent<TextComponent>(m_textComponent); // Add the pre-created TextComponent instance
        }
    }


    void FPSComponent::Update(float deltaTime)
    {
        m_deltaTimeSum += deltaTime;
        m_frameCount++;

        if (m_deltaTimeSum >= 1.0f)
        {
            m_fps = static_cast<float>(m_frameCount) / m_deltaTimeSum;
            m_deltaTimeSum = 0.0f;
            m_frameCount = 0;

            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << "FPS: " << m_fps;
            m_textComponent->SetText(stream.str());
        }
        m_textComponent->Update(deltaTime); // Update the TextComponent as well.
    }

    void FPSComponent::Render() const
    {
        // TextComponent will handle rendering
        // m_textComponent->Render(); // Optionally call render here, or rely on GameObject's render loop to render all components.
    }

}