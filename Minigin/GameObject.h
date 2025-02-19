#pragma once
#include <memory>
#include <vector>
#include <typeinfo> // Required for typeid
#include <typeindex> // Required for type_index
#include <map>
#include <queue>
//#include "Transform.h"  // No longer needed directly in GameObject
#include "BaseComponent.h"
#include "TransformComponent.h"
namespace dae
{
	class BaseComponent; // Forward declaration

	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Component Management
		template <typename T>
		void AddComponent(std::shared_ptr<T> component);

		template <typename T>
		std::shared_ptr<T> GetComponent() const;

		template <typename T>
		bool HasComponent() const;

		template <typename T>
		void RemoveComponent();

		//const Transform& GetTransform() const { return m_transform; } // Add a getter for the Transform
		//Transform& GetTransform() { return m_transform; } // And a non-const version
		TransformComponent* GetTransform() { return GetComponent<TransformComponent>().get(); }
		const TransformComponent* GetTransform() const { return GetComponent<TransformComponent>().get(); }


	private:
		//Transform m_transform;
		std::vector<std::shared_ptr<BaseComponent>> m_components; // Store base class pointers
		std::map<std::type_index, std::shared_ptr<BaseComponent>> m_componentMap;
		std::vector<std::shared_ptr<BaseComponent>> m_componentsToRemove;
		std::queue<std::type_index> m_componentTypesToRemove;
	};

	// Put the template function definitions in the header file.
	template <typename T>
	void GameObject::AddComponent(std::shared_ptr<T> component)
	{
		// Check if a component of this type already exists
		if (HasComponent<T>())
		{
			return; // Or throw an exception, depending on desired behavior
		}

		m_components.push_back(component);
		m_componentMap[typeid(T)] = component;
		//component->SetGameObject(this); // Important: Set the GameObject pointer // REMOVE
		//Call base class constructor
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent() const
	{
		auto it = m_componentMap.find(typeid(T));
		if (it != m_componentMap.end()) {
			// Cast to the requested type using std::dynamic_pointer_cast.  Safer than static_cast.
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr; // Or consider throwing an exception if it should exist.
	}

	template <typename T>
	bool GameObject::HasComponent() const
	{
		return m_componentMap.count(typeid(T)) > 0;
	}

	template <typename T>
	void GameObject::RemoveComponent()
	{
		auto typeIndex = typeid(T);
		if (m_componentMap.count(typeIndex) > 0)
		{
			auto component = m_componentMap[typeIndex];
			m_componentsToRemove.push_back(component);
			m_componentTypesToRemove.push(typeIndex);
		}
	}
}