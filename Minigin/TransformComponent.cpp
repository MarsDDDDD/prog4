// TransformComponent.cpp
#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
}

dae::TransformComponent::TransformComponent(GameObject* pOwner, const glm::vec3& position) : BaseComponent(pOwner), m_LocalPosition(position)
{
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position) {
    m_LocalPosition = position;
    SetPositionDirty();
}
void dae::TransformComponent::SetLocalPosition(float x, float y, float z) {
    m_LocalPosition = glm::vec3(x, y, z);
    SetPositionDirty();
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
    if (m_pGameObject->GetParent() == nullptr)
    {
        m_WorldPosition = m_LocalPosition;
    }
    else
    {
        // Get the parent's world position  <-  THIS WAS THE KEY FIX
        const auto parentWorldPos = m_pGameObject->GetParent()->GetTransform()->GetWorldPosition();  //  RECURSIVE CALL!
        m_WorldPosition = parentWorldPos + m_LocalPosition;
    }
    m_IsDirty = false;

    // Set children dirty
    for (auto& child : m_pGameObject->GetChildren())
    {
        child->GetTransform()->SetPositionDirty();
    }
}