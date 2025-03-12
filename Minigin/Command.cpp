#include "Command.h"
#include "InputManager.h"
#include <iostream>

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
