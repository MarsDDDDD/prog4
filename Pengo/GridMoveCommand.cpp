#include "GridMoveCommand.h"

namespace dae
{
    void GridMoveCommand::Execute(float /*deltaTime*/)
    {
        if (!m_Entity || !m_GridObject)
            return;

        auto gridComponent = m_GridObject->GetComponent<NewGridComponent>();
        if (gridComponent) {
            gridComponent->MoveEntity(m_Entity, m_Direction);
        }
    }
}