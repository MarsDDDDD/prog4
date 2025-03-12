#include "BaseComponent.h"


dae::GameObject* dae::BaseComponent::GetOwner() const
{
	return m_pOwner;
}




void dae::BaseComponent::SetOwner(GameObject* pParent)

{

	m_pOwner = pParent;

}

