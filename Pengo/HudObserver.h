#pragma once
#include "Observer.h"
#include <string>

namespace dae
{
    class GameObject;
    class TextComponent;

    class HudObserver final : public Observer
    {
    public:
        HudObserver() = default;
        ~HudObserver() override = default;
        
        void Notify(const GameObject* actor, EventId event) override;
        void SetActor(GameObject* actor);
        // Set the GameObjects containing TextComponents
        void SetHealthDisplay(GameObject* healthDisplay);
        void SetScoreDisplay(GameObject* scoreDisplay);

    private:
        GameObject* m_pActor{ nullptr };
        GameObject* m_pHealthDisplay{ nullptr };
        GameObject* m_pScoreDisplay{ nullptr };
        
        // Helper methods
        void UpdateHealthText(const GameObject* actor);
        void UpdateScoreText(const GameObject* actor);
    };
}
