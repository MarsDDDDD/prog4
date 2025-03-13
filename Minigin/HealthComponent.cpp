#include "HealthComponent.h"
#include "TextComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "GameObject.h"
#include <iostream>

namespace dae
{
	HealthComponent::HealthComponent(GameObject* pOwner)
		: BaseComponent(pOwner)
	{
		//TODO: Link to a life display
	}

	void HealthComponent::Update(float /*deltaTime*/)
	{
		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::to_string(m_CurrentHealth));
		}
	}

	void HealthComponent::SetMaxHealth(int amount)
	{
		m_MaxHealth = amount;
	}

	void HealthComponent::SetHealth(int amount)
	{
		m_CurrentHealth = amount;
		HealthUpdated();
	}

	int HealthComponent::GetHealth() const
	{
		return m_CurrentHealth;
	}

	void HealthComponent::DoDamage(int amount)
	{
		m_CurrentHealth -= amount;
		HealthUpdated();
	}

	void HealthComponent::SetTextComponent(TextComponent* pTextComponent)
	{
		m_pTextComponent = pTextComponent;
	}

	void  HealthComponent::HealthUpdated()
	{
		if (m_CurrentHealth <= 0)
		{
			m_CurrentHealth = 0;
			Subject::GetInstance().Notify(GetGameObject(), Observer::EventId::ACTOR_DIED);
		}
		else
		{
			Subject::GetInstance().Notify(GetGameObject(), Observer::EventId::HEALTH_UPDATED);
		}
		std::cout << "Current health: " << m_CurrentHealth << std::endl;

		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::to_string(m_CurrentHealth));
		}
	}

}
