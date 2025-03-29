#pragma once
#include "Command.h"
#include "GameObject.h"
#include "GridComponent.h"

namespace dae
{
    class GridMoveCommand final : public Command
    {
    public:
        explicit GridMoveCommand(GameObject* pGameObject, Direction direction)
            : m_pGameObject(pGameObject)
            , m_Direction(direction)
        {
        }

        void Execute(float deltaTime) override;

    private:
        GameObject* m_pGameObject{};
        Direction m_Direction{};
    };
}
