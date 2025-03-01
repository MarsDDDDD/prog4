#pragma once
#include "BaseComponent.h"
#include <string>
#include <memory>
#include "Font.h"
#include "Texture2D.h"
#include <SDL_ttf.h> // Include for SDL_Color

namespace dae
{
	class GameObject;
	class TextComponent : public BaseComponent
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, SDL_Color color = { 255, 255, 255, 255 }); // Add GameObject
		~TextComponent() override = default;

		void Update(float deltaTime) override;
		void Render() override;

		void SetText(const std::string& text);
		void SetColor(SDL_Color color);
		void SetFont(std::shared_ptr<Font> font);

	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
		SDL_Color m_color;

		void CreateTexture(); // Helper function
	};
}