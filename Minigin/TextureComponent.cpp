#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h" // For GetTransform

dae::TextureComponent::TextureComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture) // Add GameObject*
	: BaseComponent(pOwner), // Call base class constructor
	m_texture(std::move(texture))
{
	if (m_texture)
	{
		glm::ivec2 size = m_texture->GetSize();
		m_dimensions.x = static_cast<float>(size.x);
		m_dimensions.y = static_cast<float>(size.y);
	}
}


void dae::TextureComponent::Render() const
{
	if (m_texture)
	{
		const auto& pos = m_pGameObject->GetTransform()->GetWorldPosition(); // Correctly uses the TransformComponent
		if (m_dimensions.x != 0 && m_dimensions.y != 0)
		{
			dae::Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, m_dimensions.x, m_dimensions.y);
		}
		else
		{
			dae::Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}
}
void dae::TextureComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_texture = std::move(texture);
	if (m_texture)
	{
		glm::ivec2 size = m_texture->GetSize();
		m_dimensions.x = static_cast<float>(size.x);
		m_dimensions.y = static_cast<float>(size.y);
	}
}

void dae::TextureComponent::SetDimensions(float width, float height)
{
	m_dimensions.x = width;
	m_dimensions.y = height;
}