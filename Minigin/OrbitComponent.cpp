#include "OrbitComponent.h"
#include "GameObject.h"
#include <gtc/constants.hpp>
#include <iostream> // For debug logging

dae::OrbitComponent::OrbitComponent(GameObject* pOwner, GameObject* pTarget, float radius, float speed)
    : BaseComponent(pOwner), m_pTarget(pTarget), m_Radius(radius), m_Speed(speed), m_Angle(0.0f)
{
    if (!m_pTarget)
    {
        std::cerr << "OrbitComponent: Target GameObject is null!" << std::endl;
    }
}

void dae::OrbitComponent::Update(float deltaTime)
{
    if (!m_pTarget)
    {
        std::cerr << "OrbitComponent: Target GameObject is null during update!" << std::endl;
        return;
    }

    auto targetTransform = m_pTarget->GetTransform();
    if (!targetTransform)
    {
        std::cerr << "OrbitComponent: Target TransformComponent is null during update!" << std::endl;
        return;
    }

    m_Angle += m_Speed * deltaTime;
    if (m_Angle > glm::two_pi<float>())
    {
        m_Angle -= glm::two_pi<float>();
    }

    float x = targetTransform->GetLocalPosition().x + m_Radius * cos(m_Angle);
    float y = targetTransform->GetLocalPosition().y + m_Radius * sin(m_Angle);

    GetGameObject()->GetTransform()->SetLocalPosition(x, y, 0.0f);
}
