#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "GameObject.h"
#include <iostream>

namespace dae
{
	ScoreComponent::ScoreComponent(GameObject* pOwner)
		: BaseComponent(pOwner)
	{
		//TODO: Link to a life display
	}

	void ScoreComponent::Update(float /*deltaTime*/)
	{
		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::to_string(m_CurrentScore));
		}
	}

	void ScoreComponent::SetScore(int amount)
	{
		m_CurrentScore = amount;
		ScoreUpdated();
	}

	int ScoreComponent::GetScore() const
	{
		return m_CurrentScore;
	}

	void ScoreComponent::AddScore(int amount)
	{
		m_CurrentScore += amount;
		ScoreUpdated();
	}

	void ScoreComponent::SetTextComponent(TextComponent* pTextComponent)
	{
		m_pTextComponent = pTextComponent;
	}

	void  ScoreComponent::ScoreUpdated()
	{
		if (m_CurrentScore <= 0)
		{
			m_CurrentScore = 0;
			Subject::GetInstance().Notify(GetGameObject(), Observer::EventId::ACTOR_DIED);
		}
		else
		{
			Subject::GetInstance().Notify(GetGameObject(), Observer::EventId::SCORE_UPDATED);
		}
		std::cout << "Current Score: " << m_CurrentScore << std::endl;

		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::to_string(m_CurrentScore));
		}
	}

}
