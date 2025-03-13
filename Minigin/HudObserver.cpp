#include "HudObserver.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include <string>
#include <iostream>

namespace dae
{
    void HudObserver::Notify(const GameObject* actor, EventId event)
    {
        if (actor != m_pActor)
        {
            return;
        }

        switch(event)
        {
            case EventId::HEALTH_UPDATED:
                UpdateHealthText(actor);
                break;
                
            case EventId::SCORE_UPDATED:
                UpdateScoreText(actor);
                break;
                
            case EventId::ACTOR_DIED:
                // Could handle special display for death if needed
                UpdateHealthText(actor);
                break;
                
            default:
                break;
        }
    }

    void HudObserver::SetActor(GameObject* actor)
    {
		m_pActor = actor;
    }
    
    void HudObserver::SetHealthDisplay(GameObject* healthDisplay)
    {
        m_pHealthDisplay = healthDisplay;
    }
    
    void HudObserver::SetScoreDisplay(GameObject* scoreDisplay)
    {
        m_pScoreDisplay = scoreDisplay;
    }
    
    void HudObserver::UpdateHealthText(const GameObject* actor)
    {
        if (!m_pHealthDisplay)
        {
            std::cout << "Hud Observer: no HealthDisplay!" << std::endl;
            return;
        }

            
        auto* textComponent = m_pHealthDisplay->GetComponent<TextComponent>();
        if (!textComponent)
            return;
            
        const auto* healthComponent = actor->GetComponent<HealthComponent>();
        if (!healthComponent)
            return;
            
        std::string healthText = "Health: " + std::to_string(healthComponent->GetHealth());
        textComponent->SetText(healthText);
    }
    
    void HudObserver::UpdateScoreText(const GameObject* actor)
    {
        if (!m_pScoreDisplay)
            return;
            
        auto* textComponent = m_pScoreDisplay->GetComponent<TextComponent>();
        if (!textComponent)
            return;
            
        const auto* scoreComponent = actor->GetComponent<ScoreComponent>();
        if (!scoreComponent)
            return;
            
        std::string scoreText = "Score: " + std::to_string(scoreComponent->GetScore());
        textComponent->SetText(scoreText);
    }
}
