#include <algorithm>
#include "GameObject.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
    GameObject::GameObject()
    {
        // Create TransformComponent with unique ownership
        auto transform = std::make_unique<TransformComponent>(this);
        AddComponent(std::move(transform));
    }

    GameObject::~GameObject() = default;

    void GameObject::Update(float deltaTime)
    {
        // Update all components
        for (auto& component : m_Components)
        {
            component->Update(deltaTime);
        }

        // Apply pending removals
        for (auto* componentToRemove : m_pComponentsToRemove)
        {
            // Find this pointer in m_Components
            auto it = std::find_if(
                m_Components.begin(),
                m_Components.end(),
                [componentToRemove](std::unique_ptr<BaseComponent>& c) {
                    return c.get() == componentToRemove;
                }
            );

            // Remove from vector if found
            if (it != m_Components.end())
            {
                m_Components.erase(it);
            }

            // Also remove from the lookup map
            auto typeIndex = m_ComponentTypesToRemove.front();
            m_ComponentMap.erase(typeIndex);
            m_ComponentTypesToRemove.pop();
        }
        m_pComponentsToRemove.clear();

        // Update children
        for (const auto& child : m_Children)
        {
            child->Update(deltaTime);
        }
    }

    void GameObject::FixedUpdate(float fixedTimeStep)
    {
        for (auto& component : m_Components)
        {
            component->FixedUpdate(fixedTimeStep);
        }

        for (const auto& child : m_Children)
        {
            child->FixedUpdate(fixedTimeStep);
        }
    }

    void GameObject::Render() const
    {
        for (auto& component : m_Components)
        {
            component->Render();
        }

        for (const auto& child : m_Children)
        {
            child->Render();
        }
    }

    void GameObject::SetLocalPosition(float x, float y)
    {
        // Directly call the TransformComponent since it's always present
        if (auto* transform = GetTransform())
        {
            transform->SetLocalPosition(x, y, 0.0f);
        }
    }

    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        // Prevent self-parenting or cyclical references
        if (parent == this || (parent && parent->IsDescendant(this)))
        {
            return;
        }

        glm::vec3 originalWorldPosition{};
        if (keepWorldPosition && GetTransform())
        {
            originalWorldPosition = GetTransform()->GetWorldPosition();
        }

        if (auto currentParent = m_parent.lock())
        {
            auto& siblings = currentParent->m_Children;
            const auto it = std::find(siblings.begin(), siblings.end(), shared_from_this());
            if (it != siblings.end())
            {
                siblings.erase(it);
            }
        }

        m_parent = parent ? parent->shared_from_this() : nullptr;
        if (parent)
        {
            parent->m_Children.push_back(shared_from_this());
        }

        if (parent && keepWorldPosition)
        {
            glm::vec3 parentWorldPosition = parent->GetTransform()->GetWorldPosition();
            glm::vec3 newLocalPosition = originalWorldPosition - parentWorldPosition;
            GetTransform()->SetLocalPosition(newLocalPosition);
        }
        else if (GetTransform())
        {
            GetTransform()->SetPositionDirty();
        }
    }

    void GameObject::RemoveAllChildren()
    {
        for (auto& child : m_Children)
        {
            child->SetParent(nullptr);
        }
        m_Children.clear();
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        if (!child || !IsDescendant(child))
        {
            return;
        }

        auto it = std::remove_if(
            m_Children.begin(),
            m_Children.end(),
            [child](const std::shared_ptr<GameObject>& c) { return c.get() == child; }
        );

        if (it != m_Children.end())
        {
            m_Children.erase(it, m_Children.end());
        }

        child->m_parent.reset();

        // Recalculate child's local position based on its previous world position
        if (auto* childTransform = child->GetTransform())
        {
            glm::vec3 childWorldPosition = childTransform->GetWorldPosition();
            childTransform->SetLocalPosition(childWorldPosition);
        }
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
