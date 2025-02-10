#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

namespace dae {

    TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font)
        : m_needsUpdate(true), m_text(text), m_font(font), m_textTexture(nullptr), m_position()
    { }

    void TextComponent::Update(float /*deltaTime*/)
    {
        if (m_needsUpdate)
        {
            const SDL_Color color = { 255,255,255,255 }; // White color
            const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
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
            m_needsUpdate = false;
        }
    }

    void TextComponent::Render() const
    {
        if (m_textTexture != nullptr)
        {
            Renderer::GetInstance().RenderTexture(*m_textTexture, m_position.x, m_position.y);
        }
    }


    void TextComponent::SetText(const std::string& text)
    {
        m_text = text;
        m_needsUpdate = true;
    }

    void TextComponent::SetPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
        m_position.z = 0.0f;
    }

    glm::vec3 TextComponent::GetPosition() const
    {
        return m_position;
    }

    void TextComponent::SetFont(std::shared_ptr<Font> font)
    {
        m_font = font;
        m_needsUpdate = true; // Force re-render with new font if needed.
    }

}