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

    //******** Update Children *********
    for (const auto& child : m_children)
    {
        child->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate(float fixedTimeStep)
{
    for (const auto& component : m_components)
    {
        component->FixedUpdate(fixedTimeStep);
    }

    //******** FixedUpdate Children *********
    for (const auto& child : m_children)
    {
        child->FixedUpdate(fixedTimeStep);
    }
}


void dae::GameObject::Render() const
{
    for (const auto& component : m_components)
    {
        component->Render();
    }

    //******** Render Children *********
    for (const auto& child : m_children)
    {
        child->Render();
    }
}

void dae::GameObject::SetPosition(float x, float y)
{
    // Get the TransformComponent.  If it doesn't exist, add it.
    auto transform = GetComponent<TransformComponent>();
    if (!transform)
    {
        transform = std::make_shared<TransformComponent>(this); // Pass 'this' to the constructor
        AddComponent(transform);
    }
    transform->SetPosition(x, y, 0.0f); // Use the component's method.
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    // 1. Check for validity (prevent cycles and self-parenting)
    if (parent == this || (parent && IsDescendant(parent)))
    {
        return;  // Don't allow setting self or a descendant as parent.
    }

    //Store original position to use later
    glm::vec3 originalWorldPosition{};
    if (keepWorldPosition)
        originalWorldPosition = GetTransform()->GetPosition();


    // 2. Remove from the previous parent (if any)
    if (auto currentParent = m_parent.lock())  // Use lock() to get a shared_ptr safely
    {
        auto& siblings = currentParent->m_children;
        const auto it = std::find(siblings.begin(), siblings.end(), shared_from_this());
        if (it != siblings.end())
        {
            siblings.erase(it);
        }
    }

    // 3. Set the new parent (or clear if parent is nullptr)
    m_parent = parent ? parent->shared_from_this() : nullptr; // store weak_ptr

    // 4. Add to the new parent's children list (if not nullptr)
    if (parent)
    {
        parent->m_children.push_back(shared_from_this()); // Use shared_from_this()
    }

    // 5. Calculate and set the local position based on whether to keep the world position
    if (parent && keepWorldPosition)
    {
        // Get the parent's world position
        glm::vec3 parentWorldPosition = parent->GetTransform()->GetPosition();

        // Calculate the new local position
        glm::vec3 newLocalPosition = originalWorldPosition - parentWorldPosition;

        // Set the new local position
        GetTransform()->SetPosition(newLocalPosition);
    }
}

bool dae::GameObject::IsDescendant(GameObject* potentialDescendant) const
{
    // Traverse up the hierarchy from the potential descendant.
    auto current = potentialDescendant->m_parent.lock();
    while (current)
    {
        if (current.get() == this)  // Check for direct comparison
        {
            return true; // Found 'this' as an ancestor.
        }
        current = current->m_parent.lock(); // Move up to the next ancestor
    }
    return false;
}

void dae::GameObject::RemoveAllChildren()
{
    //Set all parents to null, this will remove the gameObject from the list
    for (auto& child : m_children)
    {
        child->SetParent(nullptr);
    }
    //clear list, to be sure
    m_children.clear();
}