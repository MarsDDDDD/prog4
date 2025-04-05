#pragma once
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
    class ScoreComponent : public BaseComponent, public Subject
    {
    public:
        explicit ScoreComponent(GameObject* pOwner);
        ~ScoreComponent() override = default;

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
