#include "GridComponent.h"
#include "GameObject.h"

dae::GridComponent::GridComponent(GameObject* pOwner, int gridWidth, int gridHeight, float cellSize, float offsetX, float offsetY)
    : BaseComponent(pOwner)
    , m_GridWidth(gridWidth)
    , m_GridHeight(gridHeight)
    , m_CellSize(cellSize)
    , m_OffsetX(offsetX)
    , m_OffsetY(offsetY)
	, m_GridPosition(0, 0)
	, m_GridDestination(0, 0)
	, m_MoveTimer(0.0f)
	, m_MoveTime(0.5f)
    , m_IsMoving(false)
{
}

void dae::GridComponent::SetGridPosition(int x, int y)
{
    // Ensure position is within grid bounds
    x = std::max(0, std::min(x, m_GridWidth - 1));
    y = std::max(0, std::min(y, m_GridHeight - 1));

    m_GridPosition = { x, y };
    m_GridDestination = m_GridPosition;
    UpdateWorldPosition();
}

void dae::GridComponent::StartMove(Direction direction)
{
    if (m_IsMoving)
        return;
    
    m_GridDestination = m_GridPosition;
    
    // Calculate new position based on direction
    switch (direction)
    {
    case Direction::Up:
        if (m_GridDestination.y > 0)
            m_GridDestination.y -= 1;
        break;
    case Direction::Down:
        if (m_GridDestination.y < m_GridHeight - 1)
            m_GridDestination.y += 1;
        break;
    case Direction::Left:
        if (m_GridDestination.x > 0)
            m_GridDestination.x -= 1;
        break;
    case Direction::Right:
        if (m_GridDestination.x < m_GridWidth - 1)
            m_GridDestination.x += 1;
        break;
    }
    
    // Only start movement if destination is different
    if (m_GridDestination.x != m_GridPosition.x || m_GridDestination.y != m_GridPosition.y)
    {
        m_IsMoving = true;
        m_MoveTimer = 0.0f;
        m_CurrentDirection = direction;
    }
}

void dae::GridComponent::Update(float deltaTime)
{
    if (!m_IsMoving)
        return;
        
    m_MoveTimer += deltaTime;
    
    if (m_MoveTimer >= m_MoveTime)
    {
        // Movement complete
        m_GridPosition = m_GridDestination;
        m_IsMoving = false;
        UpdateWorldPosition();
    }
    else
    {
        // Lerp between positions
        float t = m_MoveTimer / m_MoveTime;
        glm::vec2 startPos = GridToWorld(m_GridPosition.x, m_GridPosition.y);
        glm::vec2 endPos = GridToWorld(m_GridDestination.x, m_GridDestination.y);
        glm::vec2 lerpPos = startPos + t * (endPos - startPos);
        
        if (m_pGameObject)
        {
            m_pGameObject->SetLocalPosition(lerpPos.x, lerpPos.y);
        }
    }
}

glm::vec2 dae::GridComponent::GridToWorld(int gridX, int gridY) const
{
    // Calculate the world position at the center of the grid cell with offset
    return { 
        gridX * m_CellSize + m_CellSize / 2 + m_OffsetX, 
        gridY * m_CellSize + m_CellSize / 2 + m_OffsetY 
    };
}

glm::ivec2 dae::GridComponent::WorldToGrid(float worldX, float worldY) const
{
    // Convert world coordinates to grid coordinates, accounting for offset
    int gridX = static_cast<int>((worldX - m_OffsetX) / m_CellSize);
    int gridY = static_cast<int>((worldY - m_OffsetY) / m_CellSize);

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

void dae::GridComponent::SetOffsets(float offsetX, float offsetY)
{
    m_OffsetX = offsetX;
    m_OffsetY = offsetY;
    UpdateWorldPosition();
}
