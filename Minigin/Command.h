#pragma once
#include "GameObject.h"

namespace dae
{
	class Command
	{
	public:
		explicit Command() = default;
		virtual ~Command() = default;
		Command(const Command& other) = delete;
		Command& operator=(const Command& rhs) = delete;
		Command(Command&& other) = delete;
		Command& operator=(Command&& rhs) = delete;


		virtual void Execute() = 0;
	};

	class MoveUpCommand final : public Command
	{
	public:
		explicit MoveUpCommand(GameObject* pGameObject, float speed)
			:m_pGameObject{ pGameObject }
			, m_Speed{ speed }
		{
		}

		void Execute() override;
	private:
		GameObject* m_pGameObject{};
		float m_Speed{};
	};

	class MoveDownCommand final : public Command
	{
	public:
		explicit MoveDownCommand(GameObject* pGameObject, float speed)
			:m_pGameObject{ pGameObject }
			, m_Speed{ speed }
		{
		}

		void Execute() override;
	private:
		GameObject* m_pGameObject{};
		float m_Speed{};
	};

	class MoveLeftCommand final : public Command
	{
	public:
		explicit MoveLeftCommand(GameObject* pGameObject, float speed)
			:m_pGameObject{ pGameObject }
			, m_Speed{ speed }
		{
		}

		void Execute() override;
	private:
		GameObject* m_pGameObject{};
		float m_Speed{};
	};

	class MoveRightCommand final : public Command
	{
	public:
		explicit MoveRightCommand(GameObject* pGameObject, float speed)
			:m_pGameObject{ pGameObject }
			, m_Speed{ speed }
		{
		}

		void Execute() override;
	private:
		GameObject* m_pGameObject{};
		float m_Speed{};
	};
}



// These commands need to be registered with the Input manager
// The input manager keeps track of all active commands
// Checks if the input for the command has been executed by the user
// Calls the Execute method on the registered command
// Don't forget to unregister!
