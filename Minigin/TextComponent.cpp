#include "TextComponent.h"
#include "Renderer.h"
#include "GameObject.h" // Needed for GetTransform
#include <stdexcept>

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color) // Add GameObject
	: BaseComponent(pOwner), // Call base class constructor
	m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_color(color), m_textTexture(nullptr)
{
}

void dae::TextComponent::Update(float /*deltaTime*/)  // deltaTime is unused, but good practice
{
	if (m_needsUpdate)
	{
		CreateTexture();
		m_needsUpdate = false;
	}
}


void dae::TextComponent::Render() const
{
	if (m_textTexture)
	{
		const auto& pos = m_gameObject->GetTransform()->GetPosition(); // Correctly uses the TransformComponent
		dae::Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(SDL_Color color)
{
	m_color = color;
	m_needsUpdate = true;
}
void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = std::move(font);
	m_needsUpdate = true;
}

void dae::TextComponent::CreateTexture()
{
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_textTexture = std::make_shared<Texture2D>(texture);
}