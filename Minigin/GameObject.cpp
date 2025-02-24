#include <algorithm>
#include "GameObject.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
    GameObject::GameObject()
    {
        auto transform = std::make_shared<TransformComponent>(this);
        AddComponent(transform);
    }

    GameObject::~GameObject() = default;

    void GameObject::Update(float deltaTime)
    {
        for (const auto& component : m_components)
        {
            component->Update(deltaTime);
        }

        for (const auto& component : m_componentsToRemove)
        {
            auto it = std::find(m_components.begin(), m_components.end(), component);
            if (it != m_components.end())
            {
                m_components.erase(it);
            }

            m_componentMap.erase(m_componentTypesToRemove.front());
            m_componentTypesToRemove.pop();
        }
        m_componentsToRemove.clear();

        for (const auto& child : m_children)
        {
            child->Update(deltaTime);
        }
    }

    void GameObject::FixedUpdate(float fixedTimeStep)
    {
        for (const auto& component : m_components)
        {
            component->FixedUpdate(fixedTimeStep);
        }

        for (const auto& child : m_children)
        {
            child->FixedUpdate(fixedTimeStep);
        }
    }

    void GameObject::Render() const
    {
        for (const auto& component : m_components)
        {
            component->Render();
        }

        for (const auto& child : m_children)
        {
            child->Render();
        }
    }

    void GameObject::SetLocalPosition(float x, float y)
    {
        // Directly call the TransformComponent since it always exists
        GetTransform()->SetLocalPosition(x, y, 0.0f);
    }

    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (parent == this || (parent && parent->IsDescendant(this)))
        {
            return;
        }

        glm::vec3 originalWorldPosition{};
        if (keepWorldPosition)
        {
            originalWorldPosition = GetTransform()->GetWorldPosition();
        }

        if (auto currentParent = m_parent.lock())
        {
            auto& siblings = currentParent->m_children;
            const auto it = std::find(siblings.begin(), siblings.end(), shared_from_this());
            if (it != siblings.end())
            {
                siblings.erase(it);
            }
        }

        m_parent = parent ? parent->shared_from_this() : nullptr;
        if (parent)
        {
            parent->m_children.push_back(shared_from_this());
        }

        if (parent && keepWorldPosition)
        {
            glm::vec3 parentWorldPosition = parent->GetTransform()->GetWorldPosition();
            glm::vec3 newLocalPosition = originalWorldPosition - parentWorldPosition;
            GetTransform()->SetLocalPosition(newLocalPosition);
        }
        else
        {
            GetTransform()->SetPositionDirty();
        }
    }

    void GameObject::RemoveAllChildren()
    {
        for (auto& child : m_children)
        {
            child->SetParent(nullptr);
        }
        m_children.clear();
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        if (!child || !IsDescendant(child))
        {
            return;
        }

        auto it = std::remove_if(
            m_children.begin(),
            m_children.end(),
            [child](const std::shared_ptr<GameObject>& c) { return c.get() == child; }
        );

        if (it != m_children.end())
        {
            m_children.erase(it, m_children.end());
        }

        child->m_parent.reset();

        // Always adjust child's position using its existing transform
        auto childTransform = child->GetTransform();
        auto childWorldPosition = childTransform->GetWorldPosition();
        childTransform->SetLocalPosition(childWorldPosition);
    }

    bool GameObject::IsDescendant(GameObject* potentialDescendant) const
    {
        auto current = potentialDescendant->m_parent.lock();
        while (current)
        {
            if (current.get() == this)
            {
                return true;
            }
            current = current->m_parent.lock();
        }
        return false;
    }
}
