#include "GridMoveCommand.h"

void dae::GridMoveCommand::Execute(float /*deltaTime*/)
{
    if (!m_pGameObject)
        return;
        
    auto gridComponent = m_pGameObject->GetComponent<GridComponent>();
    if (!gridComponent)
        return;
    
    // Don't start a new move if already moving
    if (!gridComponent->IsMoving())
    {
        // Start movement in the specified direction
        gridComponent->StartMove(m_Direction);
    }
}
