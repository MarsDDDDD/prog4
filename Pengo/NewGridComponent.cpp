#include "NewGridComponent.h"
#include "GameObject.h"
#include "GridEntityComponent.h"

namespace dae
{
    // Constructor
    NewGridComponent::NewGridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize, float offsetX, float offsetY)
        : BaseComponent(pOwner)
        , m_GridWidth(gridWidth)
        , m_GridHeight(gridHeight)
        , m_CellSize(cellSize)
        , m_OffsetX(offsetX)
        , m_OffsetY(offsetY)
    {
    }

    // Check if a grid cell is occupied
    bool NewGridComponent::IsOccupied(int x, int y) const
    {
        return m_GridEntities.find({ x, y }) != m_GridEntities.end();
    }

    // Get entity at a grid position
    GameObject* NewGridComponent::GetEntityAt(int x, int y) const
    {
        auto it = m_GridEntities.find({ x, y });
        return (it != m_GridEntities.end()) ? it->second : nullptr;
    }

    // Register an entity at a grid position
    bool NewGridComponent::RegisterEntity(GameObject* entity, int x, int y)
    {
        // Check bounds
        if (x < 0 || x >= m_GridWidth || y < 0 || y >= m_GridHeight)
            return false;

        // Check if position is already occupied
        if (IsOccupied(x, y))
            return false;

        // Register entity
        m_GridEntities[{x, y}] = entity;

        // Make entity a child of the grid object
        entity->SetParent(m_pGameObject, false);

        // Update entity's GridEntityComponent
        auto* gridEntityComponent = entity->GetComponent<GridEntityComponent>();
        if (gridEntityComponent) {
            gridEntityComponent->SetGridPosition(x, y);
        }

        // Update entity position
        glm::vec2 worldPos = GridToWorld(x, y);
        entity->SetLocalPosition(worldPos.x, worldPos.y);

        return true;
    }

    // Unregister an entity from the grid
    void NewGridComponent::UnregisterEntity(GameObject* entity)
    {
        // Find the entity in the grid
        for (auto it = m_GridEntities.begin(); it != m_GridEntities.end(); ++it) {
            if (it->second == entity) {
                // Remove from grid entities map
                m_GridEntities.erase(it);
                
                // Remove parent relationship
                entity->SetParent(nullptr, true);
                
                return;
            }
        }
    }

    bool NewGridComponent::MoveEntity(GameObject* entity, Direction direction)
    {
        // Find entity's current position
        glm::ivec2 currentPos(-1, -1);
        for (const auto& pair : m_GridEntities) {
            if (pair.second == entity) {
                currentPos = { pair.first.first, pair.first.second };
                break;
            }
        }

        if (currentPos.x == -1) // Entity not found
            return false;

        // Check if entity is already moving (skip if already in movement queue)
        for (const auto& movement : m_MovingEntities) {
            if (movement.entity == entity) {
                return false; // Entity is already moving
            }
        }

        // Calculate target position
        glm::ivec2 targetPos = currentPos;
        switch (direction) {
        case Direction::Up:    targetPos.y--; break;
        case Direction::Down:  targetPos.y++; break;
        case Direction::Left:  targetPos.x--; break;
        case Direction::Right: targetPos.x++; break;
        }

        // Check boundaries
        if (targetPos.x < 0 || targetPos.x >= m_GridWidth ||
            targetPos.y < 0 || targetPos.y >= m_GridHeight)
            return false;

        // Check collision
        if (IsOccupied(targetPos.x, targetPos.y))
            return false;

        // Update grid state
        m_GridEntities.erase({ currentPos.x, currentPos.y });
        m_GridEntities[{targetPos.x, targetPos.y}] = entity;

        // Update entity's GridEntityComponent
        auto* gridEntityComponent = entity->GetComponent<GridEntityComponent>();
        if (gridEntityComponent) {
            gridEntityComponent->SetGridPosition(targetPos.x, targetPos.y);
        }

        // Start movement interpolation
        MovementData movement;
        movement.entity = entity;
        movement.startPos = currentPos;
        movement.endPos = targetPos;
        movement.timer = 0.0f;
        movement.duration = 0.5f; // Same as your original time
        movement.direction = direction;

        m_MovingEntities.push_back(movement);

        return true;
    }

    void NewGridComponent::Update(float deltaTime)
    {
        // Process all moving entities
        for (auto it = m_MovingEntities.begin(); it != m_MovingEntities.end();) {
            auto& movement = *it;
            movement.timer += deltaTime;

            if (movement.timer >= movement.duration) {
                // Movement complete
                glm::vec2 worldPos = GridToWorld(movement.endPos.x, movement.endPos.y);
                movement.entity->SetLocalPosition(worldPos.x, worldPos.y);
                
                it = m_MovingEntities.erase(it);
            }
            else {
                // Interpolate position
                float t = movement.timer / movement.duration;
                glm::vec2 startWorldPos = GridToWorld(movement.startPos.x, movement.startPos.y);
                glm::vec2 endWorldPos = GridToWorld(movement.endPos.x, movement.endPos.y);
                glm::vec2 lerpPos = startWorldPos + t * (endWorldPos - startWorldPos);

                movement.entity->SetLocalPosition(lerpPos.x, lerpPos.y);
                ++it;
            }
        }
    }
    
    // Convert grid coordinates to world coordinates
    glm::vec2 NewGridComponent::GridToWorld(int gridX, int gridY) const
    {
        return glm::vec2(
            gridX * m_CellSize + m_OffsetX,
            gridY * m_CellSize + m_OffsetY
        );
    }

    // Convert world coordinates to grid coordinates
    glm::ivec2 NewGridComponent::WorldToGrid(float worldX, float worldY) const
    {
        return glm::ivec2(
            static_cast<int>(worldX / m_CellSize),
            static_cast<int>(worldY / m_CellSize)
        );
    }
}