#include <string>
#include "GameObject.h"
#include "BaseComponent.h" // Include the BaseComponent header
#include <algorithm>

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
	m_transform.SetPosition(x, y, 0.0f);
}

template <typename T>
void dae::GameObject::AddComponent(std::shared_ptr<T> component)
{
	// Check if a component of this type already exists
	if (HasComponent<T>())
	{
		return; // Or throw an exception, depending on desired behavior
	}

	m_components.push_back(component);
	m_componentMap[typeid(T)] = component;
	component->SetGameObject(this); // Important: Set the GameObject pointer
}

template <typename T>
std::shared_ptr<T> dae::GameObject::GetComponent() const
{
	auto it = m_componentMap.find(typeid(T));
	if (it != m_componentMap.end()) {
		// Cast to the requested type using std::dynamic_pointer_cast.  Safer than static_cast.
		return std::dynamic_pointer_cast<T>(it->second);
	}
	return nullptr; // Or consider throwing an exception if it *should* exist.
}

template <typename T>
bool dae::GameObject::HasComponent() const
{
	return m_componentMap.count(typeid(T)) > 0;
}

template <typename T>
void dae::GameObject::RemoveComponent()
{
	auto typeIndex = typeid(T);
	if (m_componentMap.count(typeIndex) > 0)
	{
		auto component = m_componentMap[typeIndex];
		// Remove from the vector
		auto it = std::find(m_components.begin(), m_components.end(), component);
		if (it != m_components.end())
		{
			m_components.erase(it);
		}

		m_componentMap.erase(typeIndex);
	}
}