#include "OrbitComponent.h"
#include "GameObject.h"
#include <gtc/constants.hpp>

dae::OrbitComponent::OrbitComponent(GameObject* pOwner, GameObject* pTarget, float radius, float speed)
    : BaseComponent(pOwner), m_pTarget(pTarget), m_Radius(radius), m_Speed(speed), m_Angle(0.0f)
{
}

void dae::OrbitComponent::Update(float deltaTime)
{

    if (m_pTarget == nullptr)
    {
        return; // Early exit if the target is null
    }

    m_Angle += m_Speed * deltaTime;
    if (m_Angle > glm::two_pi<float>())
    {
        m_Angle -= glm::two_pi<float>();
    }

    float x = m_pTarget->GetTransform()->GetLocalPosition().x + m_Radius * cos(m_Angle);
    float y = m_pTarget->GetTransform()->GetLocalPosition().y + m_Radius * sin(m_Angle);

    GetGameObject()->GetTransform()->SetLocalPosition(x, y, 0.0f);
}
