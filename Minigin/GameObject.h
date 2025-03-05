#pragma once
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <queue>
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
    class BaseComponent;

    class GameObject final : public std::enable_shared_from_this<GameObject>
    {
    public:

        GameObject();
        virtual ~GameObject();
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        void Update(float deltaTime);
        void FixedUpdate(float fixedTimeStep);
        void Render() const;

        // Position
        void SetLocalPosition(float x, float y);

        // Component Management
        template <typename T>
        void AddComponent(std::unique_ptr<T> component);

        template <typename T>
        T* GetComponent() const;

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        void RemoveComponent();

        TransformComponent* GetTransform() { return GetComponent<TransformComponent>(); }
        const TransformComponent* GetTransform() const { return GetComponent<TransformComponent>(); }

        // Hierarchy
        void SetParent(GameObject* parent, bool keepWorldPosition = true);
        std::weak_ptr<GameObject> GetParent() const { return m_parent; }
        const std::vector<std::shared_ptr<GameObject>>& GetChildren() const { return m_children; }
        void RemoveAllChildren();
        void RemoveChild(GameObject* child);

    private:
        // Component storage
        std::vector<std::unique_ptr<BaseComponent>> m_components;
        std::map<std::type_index, BaseComponent*> m_componentMap;

        // For deferred removal
        std::vector<BaseComponent*> m_componentsToRemove;
        std::queue<std::type_index> m_componentTypesToRemove;

        // Parent/Child
        std::weak_ptr<GameObject> m_parent;
        std::vector<std::shared_ptr<GameObject>> m_children;

        bool IsDescendant(GameObject* potentialDescendant) const;
    };

    // Template Implementations
    template <typename T>
    void GameObject::AddComponent(std::unique_ptr<T> component)
    {
        if (HasComponent<T>())
            return;

        // Store raw pointer in the map before we move it
        T* rawPtr = component.get();
        m_componentMap[typeid(T)] = rawPtr;

        // Now move into the vector
        m_components.push_back(std::move(component));
    }

    template <typename T>
    T* GameObject::GetComponent() const
    {
        auto it = m_componentMap.find(typeid(T));
        if (it != m_componentMap.end())
        {
            // Use dynamic_cast for proper downcasting
            return dynamic_cast<T*>(it->second);
        }
        return nullptr;
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
            auto ptr = m_componentMap[typeid(T)];
            // Mark for removal
            m_componentsToRemove.push_back(ptr);
            m_componentTypesToRemove.push(typeIndex);
        }
    }
}
