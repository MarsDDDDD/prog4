#pragma once
#include "BaseComponent.h"


namespace dae
{

	class ScoreComponent : public BaseComponent
	{
	public:
		explicit ScoreComponent(GameObject* pOwner);
		~ScoreComponent() override = default;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		void Update(float deltaTime) override;

		//Setters
		void SetScore(int newScore);

		//Getters
		int GetScore() const;

		void AddScore(int amount);


	private:

		void ScoreUpdated();


		int m_CurrentScore{ 0 };

	};

}
