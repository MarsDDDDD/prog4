#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
#include "Direction.h"
#include <map>
#include <vector>

namespace dae
{

    class NewGridComponent : public BaseComponent
    {
    public:
        NewGridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize, float offsetX = 0.5f, float offsetY = 0.5f);
        ~NewGridComponent() override = default;

        // Grid state management
        bool IsOccupied(int x, int y) const;
        GameObject* GetEntityAt(int x, int y) const;
        bool RegisterEntity(GameObject* entity, int x, int y);
        void UnregisterEntity(GameObject* entity);

        // Movement
        bool MoveEntity(GameObject* entity, Direction direction);
        void Update(float deltaTime) override;

        // Grid info and conversion
        int GetGridWidth() const { return m_GridWidth; }
        int GetGridHeight() const { return m_GridHeight; }
        float GetCellSize() const { return m_CellSize; }
        glm::vec2 GridToWorld(int gridX, int gridY) const;
        glm::ivec2 WorldToGrid(float worldX, float worldY) const;

    private:
        int m_GridWidth;
        int m_GridHeight;
        float m_CellSize;
        float m_OffsetX;
        float m_OffsetY;

        // Grid state storage - map of positions to objects
        std::map<std::pair<int, int>, GameObject*> m_GridEntities;

        // Movement tracking
        struct MovementData {
            GameObject* entity;
            glm::ivec2 startPos;
            glm::ivec2 endPos;
            float timer;
            float duration;
            Direction direction;
        };
        std::vector<MovementData> m_MovingEntities;
    };

}