#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Observer.h"

namespace dae
{
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
