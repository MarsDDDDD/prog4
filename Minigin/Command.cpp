#include "Command.h"
#include "InputManager.h"
#include <iostream>

using namespace dae;

void MoveUpCommand::Execute(float deltaTime)
{
	std::cout << "up" << std::endl;

	if (m_pGameObject && m_pGameObject->GetTransform())
	{
		auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
		currentPos.y -= m_Speed * deltaTime;
		m_pGameObject->GetTransform()->SetLocalPosition(currentPos);
	}
}

void MoveDownCommand::Execute(float deltaTime)
{
	std::cout << "down" << std::endl;
	if (m_pGameObject && m_pGameObject->GetTransform())
	{
		auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
		currentPos.y += m_Speed * deltaTime;
		m_pGameObject->GetTransform()->SetLocalPosition(currentPos);
	}

}

void MoveLeftCommand::Execute(float deltaTime)
{
	std::cout << "left" << std::endl;
	if (m_pGameObject && m_pGameObject->GetTransform())
	{
		auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
		currentPos.x -= m_Speed * deltaTime;
		m_pGameObject->GetTransform()->SetLocalPosition(currentPos);
	}

}

void MoveRightCommand::Execute(float deltaTime)
{
	std::cout << "right" << std::endl;
	if (m_pGameObject && m_pGameObject->GetTransform())
	{
		auto currentPos = m_pGameObject->GetTransform()->GetLocalPosition();
		currentPos.x += m_Speed * deltaTime;
		m_pGameObject->GetTransform()->SetLocalPosition(currentPos);
	}

}
