#include "Command.h"
#include "InputManager.h"
#include <iostream>
#include "Subject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace dae;

void MoveCommand::Execute(float deltaTime)
{
	switch (m_Direction)
	{
	case Direction::Up:
		std::cout << "up" << std::endl;
		break;
	case Direction::Down:
		std::cout << "down" << std::endl;
		break;
	case Direction::Left:
		std::cout << "left" << std::endl;
		break;
	case Direction::Right:
		std::cout << "right" << std::endl;
		break;
	}

	if (m_pGameObject && m_pGameObject->GetTransform())
	{
		auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
		switch (m_Direction)
		{
		case Direction::Up:
			currentPos.y -= m_Speed * deltaTime;
			break;
		case Direction::Down:
			currentPos.y += m_Speed * deltaTime;
			break;
		case Direction::Left:
			currentPos.x -= m_Speed * deltaTime;
			break;
		case Direction::Right:
			currentPos.x += m_Speed * deltaTime;
			break;
		}
		m_pGameObject->GetTransform()->SetLocalPosition(currentPos);
	}
}

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

	Subject::GetInstance().Notify(m_pGameObject, m_EventId);
}
