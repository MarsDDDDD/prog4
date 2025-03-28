#pragma once
#include "Command.h"
#include "GameObject.h"

namespace dae
{
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};

	class MoveCommand final : public Command
	{
	public:
		explicit MoveCommand(GameObject* pGameObject, float speed, Direction direction)
			: m_pGameObject{ pGameObject }
			, m_Speed{ speed }
			, m_Direction{ direction }
		{
		}

		void Execute(float deltaTime) override;

	private:
		GameObject* m_pGameObject{};
		float m_Speed{};
		Direction m_Direction{};
	};
}
