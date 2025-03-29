#include "GridMoveCommand.h"

void dae::GridMoveCommand::Execute(float /*deltaTime*/)
{
    if (!m_pGameObject)
        return;
        
    auto gridComponent = m_pGameObject->GetComponent<GridComponent>();
    if (!gridComponent)
        return;
        
    // Get current grid position
    auto currentPos = gridComponent->GetGridPosition();
    
    // Calculate new position based on direction
    switch (m_Direction)
    {
    case Direction::Up:
        if (currentPos.y > 0)
            currentPos.y -= 1;
        break;
    case Direction::Down:
        if (currentPos.y < gridComponent->GetGridHeight() - 1)
            currentPos.y += 1;
        break;
    case Direction::Left:
        if (currentPos.x > 0)
            currentPos.x -= 1;
        break;
    case Direction::Right:
        if (currentPos.x < gridComponent->GetGridWidth() - 1)
            currentPos.x += 1;
        break;
    }
    
    // Update grid position (which will update world position)
    gridComponent->SetGridPosition(currentPos.x, currentPos.y);
}
