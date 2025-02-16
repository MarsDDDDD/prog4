#include <string>
#include "GameObject.h"
#include "BaseComponent.h"
#include <algorithm>
#include "TransformComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
    for (const auto& component : m_components)
    {
        component->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate(float fixedTimeStep)
{
    for (const auto& component : m_components)
    {
        component->FixedUpdate(fixedTimeStep);
    }
}


void dae::GameObject::Render() const
{
    for (const auto& component : m_components)
    {
        component->Render();
    }
}

void dae::GameObject::SetPosition(float x, float y)
{
    // Get the TransformComponent.  If it doesn't exist, add it.
    auto transform = GetComponent<TransformComponent>();
    if (!transform)
    {
        transform = std::make_shared<TransformComponent>();
        AddComponent(transform);
    }
    transform->SetPosition(x, y, 0.0f); // Use the component's method.
}
