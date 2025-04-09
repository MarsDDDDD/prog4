#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
#include "Direction.h"

namespace dae
{
    class GridComponent : public BaseComponent
    {
    public:
        GridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize, float offsetX = 0.0f, float offsetY = 0.0f);
        ~GridComponent() override = default;

        // Grid position
        void SetGridPosition(int x, int y);
        glm::ivec2 GetGridPosition() const { return m_GridPosition; }

        // Movement
        void StartMove(Direction direction);
        void Update(float deltaTime) override;
        bool IsMoving() const { return m_IsMoving; }

        // Convert between grid and world coordinates
        glm::vec2 GridToWorld(int gridX, int gridY) const;
        glm::ivec2 WorldToGrid(float worldX, float worldY) const;

        // Grid dimensions
        int GetGridWidth() const { return m_GridWidth; }
        int GetGridHeight() const { return m_GridHeight; }
        float GetCellSize() const { return m_CellSize; }

        // Update the GameObject's position based on grid position
        void UpdateWorldPosition();
        
        // Offsets
        void SetOffsets(float offsetX, float offsetY);

    private:
        int m_GridWidth;
        int m_GridHeight;
        float m_CellSize;
        float m_OffsetX{0.0f};
        float m_OffsetY{0.0f};
        glm::ivec2 m_GridPosition{ 0, 0 };
        glm::ivec2 m_GridDestination{ 0, 0 };
        float m_MoveTimer{ 0.0f };
		// seconds needed to move from one cell to another
        float m_MoveTime{ 0.5f };
        bool m_IsMoving{ false };
        Direction m_CurrentDirection{};
    };
}
