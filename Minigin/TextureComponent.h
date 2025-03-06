// ===== TextureComponent.h =====
#pragma once
#include "BaseComponent.h"
#include <string>
#include <memory>
#include "Texture2D.h"

namespace dae
{
	class GameObject;
	class TextureComponent : public BaseComponent
	{
	public:
		TextureComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture); // Add GameObject

		~TextureComponent() override = default;

		void Render() const override;
		void SetTexture(std::shared_ptr<Texture2D> texture);

		// Optional: Add methods to control scaling, rotation, etc. if needed.
		void SetDimensions(float width, float height);
		const glm::vec2& GetDimensions() const { return m_dimensions; };

	private:
		std::shared_ptr<Texture2D> m_texture;
		glm::vec2 m_dimensions{};
	};
}