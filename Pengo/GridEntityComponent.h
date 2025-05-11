#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
#include "Direction.h"

namespace dae
{
    class GridEntityComponent : public BaseComponent
    {
    public:
        GridEntityComponent(GameObject* pOwner) : BaseComponent(pOwner) {}

        glm::ivec2 GetGridPosition() const { return m_GridPosition; }
        void SetGridPosition(int x, int y) { m_GridPosition = { x, y }; }

    private:
        glm::ivec2 m_GridPosition{ -1, -1 };
        friend class NewGridComponent;
    };
}