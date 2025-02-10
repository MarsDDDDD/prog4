#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Font.h"

namespace dae {
    class TextObject;

    class TextComponent final : public Component
    {
    public:
        TextComponent(const std::string& text, std::shared_ptr<Font> font);
        ~TextComponent() override = default;

        void Update(float deltaTime) override;
        void Render() const; // Add render method for direct rendering if needed

        void SetText(const std::string& text);
        void SetPosition(float x, float y);

        glm::vec3 GetPosition() const;
        void SetFont(std::shared_ptr<Font> font);

    private:
        bool m_needsUpdate;
        std::string m_text;
        glm::vec3 m_position;
        std::shared_ptr<Font> m_font;
        std::shared_ptr<Texture2D> m_textTexture;
    };
}