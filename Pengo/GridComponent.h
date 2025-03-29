#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

namespace dae
{
    class GridComponent : public BaseComponent
    {
    public:
        GridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize);
        ~GridComponent() override = default;

        // Grid position
        void SetGridPosition(int x, int y);
        glm::ivec2 GetGridPosition() const { return m_GridPosition; }

        // Convert between grid and world coordinates
        glm::vec2 GridToWorld(int gridX, int gridY) const;
        glm::ivec2 WorldToGrid(float worldX, float worldY) const;

        // Grid dimensions
        int GetGridWidth() const { return m_GridWidth; }
        int GetGridHeight() const { return m_GridHeight; }
        float GetCellSize() const { return m_CellSize; }

        // Update the GameObject's position based on grid position
        void UpdateWorldPosition();

    private:
        int m_GridWidth;
        int m_GridHeight;
        float m_CellSize;
        glm::ivec2 m_GridPosition{ 0, 0 };
    };
}
