// OrbitComponent.cpp
#include "OrbitComponent.h"
#include "TransformComponent.h"
#include "GameObject.h" // Add this include to resolve the undefined type error
#include <cmath>

dae::OrbitComponent::OrbitComponent(GameObject* pOwner, float radius, float speed)
    : BaseComponent(pOwner)
    , m_Radius(radius)
    , m_Speed(speed)
    , m_Angle(0.0f)
{
}

void dae::OrbitComponent::Update(float deltaTime)
{
    m_Angle += m_Speed * deltaTime;

    float x = m_Radius * cosf(m_Angle);
    float y = m_Radius * sinf(m_Angle);

    // Update the local position relative to the parent
    GetGameObject()->GetTransform()->SetLocalPosition(x, y, 0.0f); // Corrected: SetLocalPosition
}