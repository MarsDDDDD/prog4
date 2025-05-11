#pragma once
#include "Command.h"
#include "GameObject.h"
#include "NewGridComponent.h"

namespace dae
{
    class GridMoveCommand final : public Command
    {
    public:
        explicit GridMoveCommand(GameObject* entity, GameObject* gridObject, Direction direction)
            : m_Entity(entity)
            , m_GridObject(gridObject)
            , m_Direction(direction)
        {
        }

        void Execute(float deltaTime) override;

    private:
        GameObject* m_Entity{};
        GameObject* m_GridObject{};
        Direction m_Direction{};
    };
}
