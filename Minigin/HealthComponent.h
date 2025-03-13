#pragma once
#include "BaseComponent.h"


namespace dae
{
	class TextComponent;

	class HealthComponent : public BaseComponent
	{
	public:
		explicit HealthComponent(GameObject* pOwner);
		~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void Update(float deltaTime) override;

		//Setters
		void SetMaxHealth(int newMaxHealth);
		void SetHealth(int newHealth);
		void SetTextComponent(TextComponent* pTextComponent);

		//Getters
		int GetHealth() const;

		void DoDamage(int amount);


	private:

		void HealthUpdated();


		int m_MaxHealth{ 5 };
		int m_CurrentHealth{ 5 };
		
		TextComponent* m_pTextComponent{ nullptr };
	};

}
