#pragma once
#include <memory>
#include <vector>
#include <typeinfo> // Required for typeid
#include <typeindex> // Required for type_index
#include <map>
#include "Transform.h"

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

		const Transform& GetTransform() const { return m_transform; } // Add a getter for the Transform
		Transform& GetTransform() { return m_transform; } // And a non-const version

	private:
		Transform m_transform;
		std::vector<std::shared_ptr<BaseComponent>> m_components; // Store base class pointers
		std::map<std::type_index, std::shared_ptr<BaseComponent>> m_componentMap;
	};
}