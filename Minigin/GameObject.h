// GameObject.h

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
        GameObject(); // Changed: define constructor in .cpp to create TransformComponent
        ~GameObject();

        void Update(float deltaTime);
        void FixedUpdate(float fixedTimeStep);
        void Render() const;

        void SetLocalPosition(float x, float y);

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        template <typename T>
        void AddComponent(std::shared_ptr<T> component);

        template <typename T>
        std::shared_ptr<T> GetComponent() const;

        template <typename T>
        bool HasComponent() const;

        template <typename T>
        void RemoveComponent();

        TransformComponent* GetTransform() { return GetComponent<TransformComponent>().get(); }
        const TransformComponent* GetTransform() const { return GetComponent<TransformComponent>().get(); }

        void SetParent(GameObject* parent, bool keepWorldPosition = true);
        std::weak_ptr<GameObject> GetParent() const { return m_parent; }
        const std::vector<std::shared_ptr<GameObject>>& GetChildren() const { return m_children; }
        void RemoveAllChildren();
        void RemoveChild(GameObject* child);

    private:
        std::vector<std::shared_ptr<BaseComponent>> m_components;
        std::map<std::type_index, std::shared_ptr<BaseComponent>> m_componentMap;
        std::vector<std::shared_ptr<BaseComponent>> m_componentsToRemove;
        std::queue<std::type_index> m_componentTypesToRemove;

        std::weak_ptr<GameObject> m_parent;
        std::vector<std::shared_ptr<GameObject>> m_children;

        bool IsDescendant(GameObject* potentialDescendant) const;
    };

    template <typename T>
    void GameObject::AddComponent(std::shared_ptr<T> component)
    {
        if (HasComponent<T>()) return;
        m_components.push_back(component);
        m_componentMap[typeid(T)] = component;
    }

    template <typename T>
    std::shared_ptr<T> GameObject::GetComponent() const
    {
        auto it = m_componentMap.find(typeid(T));
        if (it != m_componentMap.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
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
            auto component = m_componentMap[typeIndex];
            m_componentsToRemove.push_back(component);
            m_componentTypesToRemove.push(typeIndex);
        }
    }
}
