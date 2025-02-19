#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
}

dae::TransformComponent::TransformComponent(GameObject* pOwner, const glm::vec3& position) : BaseComponent(pOwner), m_position(position)
{
}