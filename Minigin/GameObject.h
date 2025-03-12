#pragma once
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <queue>
#include <iostream>
#include "BaseComponent.h"
#include "TransformComponent.h"


namespace dae
{
	class BaseComponent;

	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render() const;

		GameObject();
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		//TransformComponent* GetTransform() const { return GetComponent<TransformComponent>(); }
		const TransformComponent* GetTransform() const { return GetComponent<TransformComponent>(); }
		void SetParent(GameObject* pParent);
		GameObject* GetParent() const;
		const std::vector<std::unique_ptr<GameObject>>& GetChildren() const { return m_pChildren; }

		void Destroy();
		bool IsMarkedAsDead() const { return m_IsMarkedDead; };

		GameObject* CreateGameObject();

		template <class T>
		T* AddComponent();

		template <class T>
		bool RemoveComponent();

		template <class T>
		T* GetComponent() const;

		template <class T>
		bool HasComponent() const;

	private:
		GameObject* m_pParent{};
		std::vector<std::unique_ptr<GameObject>> m_pChildren{};
		std::vector<std::unique_ptr<BaseComponent>> m_Components{};

		bool m_IsMarkedDead{};
	};




	template<class T>
	inline T* dae::GameObject::AddComponent()
	{
		static_assert(std::is_base_of<BaseComponent, T>(), "T needs to be derived from the BaseComponent class");

		if (HasComponent<T>())
		{
			std::cout << "Trying to add an already existing component\n";
			return nullptr;
		}

		std::unique_ptr<T> pComponent = std::make_unique<T>();
		pComponent->SetOwner(this);

		T* rawPtr = pComponent.get();
		m_Components.emplace_back(std::move(pComponent));

		return rawPtr;
	}

	template <class T>
	inline bool GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<BaseComponent, T>(), "T needs to be derived from the BaseComponent class");

		for (const auto& component : m_Components)
		{
			if (std::dynamic_pointer_cast<T>(component))
			{
				delete component;
				return true;
			}
		}
		return false;
	}

	template<class T>
	inline T* dae::GameObject::GetComponent() const
	{
		static_assert(std::is_base_of<BaseComponent, T>(), "T needs to be derived from the BaseComponent class");

		for (const auto& component : m_Components)
		{
			if (dynamic_cast<T*>(component.get()))
				return dynamic_cast<T*>(component.get());
		}

		return nullptr;
	}

	template <class T>
	inline bool dae::GameObject::HasComponent() const
	{
		static_assert(std::is_base_of<BaseComponent, T>(), "T needs to be derived from the BaseComponent class");

		for (const auto& component : m_Components)
		{
			if (dynamic_cast<T*>(component.get()))
				return true;
		}
		return false;
	}
}

