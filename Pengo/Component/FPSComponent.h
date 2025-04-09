#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
    // Forward-declare classes used here
    class TextComponent;

    class FPSComponent final : public BaseComponent
    {
    public:
        explicit FPSComponent(GameObject* pOwner);
        ~FPSComponent() override = default;

        void Update(float deltaTime) override;
        float GetFPS() const;

    private:
        float m_fps;
        float m_timeAccumulator;
        int m_frameCount;

        TextComponent* m_pTextComponent{ nullptr };

        // Buffer of frame times for averaging
        std::vector<float> m_frameTimes;
        const size_t m_maxSamples{ 100 };
    };
} 
