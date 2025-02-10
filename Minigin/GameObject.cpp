#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

namespace dae {

    void GameObject::Update(float deltaTime) // Modified to take deltaTime
    {
        for (const auto& component : m_components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::Render() const
    {
        const auto& pos = m_transform.GetPosition();
        if (m_texture) // Check if texture is loaded before rendering
            Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
    }

    void GameObject::SetTexture(const std::string& filename)
    {
        m_texture = ResourceManager::GetInstance().LoadTexture(filename);
    }

    void GameObject::SetPosition(float x, float y)
    {
        m_transform.SetPosition(x, y, 0.0f);
    }

}