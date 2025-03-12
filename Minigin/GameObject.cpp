#include <algorithm>
#include "GameObject.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

namespace dae
{
    GameObject::GameObject()
    {
    }

    GameObject::~GameObject() = default;


	void GameObject::Update(float deltaTime)
	{
		for (const auto& component : m_Components)
		{
			component->Update(deltaTime);
		}
		for (const auto& child : m_pChildren)
		{
			child->Update(deltaTime);
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

	}

	void GameObject::FixedUpdate(float deltaTime)
	{
		for (auto& component : m_Components)
		{
			component->FixedUpdate(deltaTime);
		}
		for (const auto& child : m_pChildren)
		{
			child->FixedUpdate(deltaTime);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& pComponent : m_Components)
		{
			pComponent->Render();
		}
		for (const auto& child : m_pChildren)
		{
			child->Render();
		}
	}

	void GameObject::SetLocalPosition(float x, float y)
	{
		// Directly call the TransformComponent since it's always present
		GetTransform()->SetLocalPosition(x, y, 0.0f);
	}

	void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
	{
		// Prevent self-parenting, setting same parent or cyclical references
		if (pParent == this || pParent == m_pParent || (pParent && pParent->IsDescendant(this)))
		{
			return;
		}

		GameObject* pOldParent{};

		if (pOldParent)
		{
			if (pOldParent == pParent) return;

			// Remove itself from the children list of the previous parent
			for (int i{ static_cast<int>(pOldParent->m_pChildren.size() - 1) }; i >= 0; --i)
			{
				const auto& pChild{ pOldParent->m_pChildren[i] };

				if (pChild.get() == this)
				{
					pOldParent->m_pChildren[i] = std::move(pOldParent->m_pChildren[pOldParent->m_pChildren.size() - 1]);
					pOldParent->m_pChildren.pop_back();
					break;
				}
			}
		}
		else if (!pParent)
		{
			return;
		}


		m_pParent = pParent;

		if (pParent)
			pParent->m_pChildren.emplace_back(std::unique_ptr<GameObject>(this));

		auto pTransform{ GetTransform() };

		if (pParent && keepWorldPosition)
		{
			glm::vec3 originalWorldPosition{ GetTransform()->GetWorldPosition() };
			glm::vec3 parentWorldPosition = pParent->GetTransform()->GetWorldPosition();
			glm::vec3 newLocalPosition = originalWorldPosition - parentWorldPosition;
			GetTransform()->SetLocalPosition(newLocalPosition);
		}
		else
		{
			GetTransform()->SetPositionDirty();
		}

	}

	GameObject* GameObject::GetParent() const
	{
		if (m_pParent) return nullptr;

		return m_pParent;
	}

	void GameObject::Destroy()
	{
		m_IsMarkedDead = true;

		// Destroy all children
		for (const auto& child : m_pChildren)
		{
			child->Destroy();
		}
	}

	GameObject* GameObject::CreateGameObject()
	{
		auto pGameObject{ std::make_unique<GameObject>() };
		// Create TransformComponent with unique ownership
		auto transform = std::make_unique<TransformComponent>(this);
		AddComponent(std::move(transform));

		const auto pGameObjectToReturn{ pGameObject.get() };

		pGameObject->m_pParent = this;
		m_pChildren.emplace_back(std::move(pGameObject));

		return pGameObjectToReturn;
	}


	bool GameObject::IsDescendant(GameObject* potentialDescendant) const
	{
		auto current = potentialDescendant->m_pParent;
		while (current)
		{
			if (current == this)
			{
				return true;
			}
			current = current->m_pParent;
		}
		return false;
	}
}
