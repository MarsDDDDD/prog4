// GridComponent.cpp
#include "GridComponent.h"
#include "GameObject.h"

dae::GridComponent::GridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize)
    : BaseComponent(pOwner)
    , m_GridWidth(gridWidth)
    , m_GridHeight(gridHeight)
    , m_CellSize(cellSize)
{
}

void dae::GridComponent::SetGridPosition(int x, int y)
{
    // Ensure position is within grid bounds
    x = std::max(0, std::min(x, m_GridWidth - 1));
    y = std::max(0, std::min(y, m_GridHeight - 1));

    m_GridPosition = { x, y };
    UpdateWorldPosition();
}

glm::vec2 dae::GridComponent::GridToWorld(int gridX, int gridY) const
{
    // Calculate the world position at the center of the grid cell
    return { gridX * m_CellSize + m_CellSize / 2, gridY * m_CellSize + m_CellSize / 2 };
}

glm::ivec2 dae::GridComponent::WorldToGrid(float worldX, float worldY) const
{
    // Convert world coordinates to grid coordinates
    int gridX = static_cast<int>(worldX / m_CellSize);
    int gridY = static_cast<int>(worldY / m_CellSize);

    // Clamp to grid bounds
    gridX = std::max(0, std::min(gridX, m_GridWidth - 1));
    gridY = std::max(0, std::min(gridY, m_GridHeight - 1));

    return { gridX, gridY };
}

void dae::GridComponent::UpdateWorldPosition()
{
    if (m_pGameObject)
    {
        // Calculate world position from grid position
        glm::vec2 worldPos = GridToWorld(m_GridPosition.x, m_GridPosition.y);

        // Update GameObject position
        m_pGameObject->SetLocalPosition(worldPos.x, worldPos.y);
    }
}
