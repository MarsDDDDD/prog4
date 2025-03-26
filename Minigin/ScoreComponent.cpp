#include "ScoreComponent.h"
#include "Observer.h"
#include "Event.h"
#include "GameObject.h"
#include <iostream>

namespace dae
{
	ScoreComponent::ScoreComponent(GameObject* pOwner)
		: BaseComponent(pOwner)
	{
	}

	void ScoreComponent::Update(float /*deltaTime*/)
	{
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


	void ScoreComponent::ScoreUpdated()
	{
		Notify(GetGameObject(), Observer::EventId::SCORE_UPDATED);
		std::cout << "Current Score: " << m_CurrentScore << std::endl;
	}

}
