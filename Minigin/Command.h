#pragma once
#include "GameObject.h"
#include "Observer.h"

namespace dae
{
	class Command
	{
	public:
		explicit Command() = default;
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute(float deltaTime = 0.f) = 0;
	};

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

	class DebugEventCommand final : public Command
	{
	public:
		explicit DebugEventCommand(GameObject* pGameObject, int amount, Observer::EventId EventId)
			: m_pGameObject{ pGameObject }
			, m_Amount{ amount }
			, m_EventId{ EventId }
		{
		}

		void Execute(float deltaTime) override;

	private:
		GameObject* m_pGameObject{};
		int m_Amount{};
		Observer::EventId m_EventId{};
	};
}
