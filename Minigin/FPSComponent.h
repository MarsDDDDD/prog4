#pragma once
#include <string>
#include <memory>
#include <chrono>
#include "Component.h"
#include "TextComponent.h"

namespace dae {

    class FPSComponent final : public Component
    {
    public:
        FPSComponent(std::shared_ptr<Font> font);
        ~FPSComponent() override = default;

        void Update(float deltaTime) override;
        void Render() const; // Optional, TextComponent can render itself

    private:
        float m_deltaTimeSum;
        int m_frameCount;
        float m_fps;
        std::shared_ptr<TextComponent> m_textComponent;
        std::chrono::high_resolution_clock::time_point m_lastTime;
    };
}