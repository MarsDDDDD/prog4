#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
}

dae::TransformComponent::TransformComponent(GameObject* pOwner, const glm::vec3& position) : BaseComponent(pOwner), m_LocalPosition(position)
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldPosition();
	}

	return m_WorldPosition;
}



void dae::TransformComponent::UpdateWorldPosition()
{
    if (m_gameObject->GetParent().lock() == nullptr)
    {
        m_WorldPosition = m_LocalPosition;
    }
    else
    {
		// Get the parent's world position
		const auto parentWorldPos = m_gameObject->GetParent().lock()->GetTransform()->GetWorldPosition();
        m_WorldPosition = parentWorldPos + m_LocalPosition;
    }
    m_IsDirty = false;
}