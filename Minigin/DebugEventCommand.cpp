#include "DebugEventCommand.h"
#include <iostream>
#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace dae;

void DebugEventCommand::Execute(float /*deltaTime*/)
{
	std::cout << "DebugEventCommand executed" << std::endl;

	switch (m_EventId)
	{
	case dae::Observer::EventId::ACTOR_DIED:
		break;
	case dae::Observer::EventId::HEALTH_UPDATED:
		m_pGameObject->GetComponent<HealthComponent>()->DoDamage(m_Amount);
		break;
	case dae::Observer::EventId::SCORE_UPDATED:
		m_pGameObject->GetComponent<ScoreComponent>()->AddScore(m_Amount);
		break;
	default:
		break;
	}
}
