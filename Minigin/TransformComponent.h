#pragma once
#include "BaseComponent.h"
#include <glm.hpp>
namespace dae
{
    class GameObject;
    class TransformComponent : public BaseComponent
    {
    public:
        TransformComponent(GameObject* pOwner);
        TransformComponent(GameObject* pOwner, const glm::vec3& position);
        ~TransformComponent() override = default;

        // Getters and Setters
        const glm::vec3& GetPosition() const { return m_position; }
        void SetPosition(const glm::vec3& position) { m_position = position; }
        void SetPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }

        // Placeholder for later:
        // const glm::quat& GetRotation() const { return m_rotation; }
        // void SetRotation(const glm::quat& rotation) { m_rotation = rotation; }
        // const glm::vec3& GetScale() const { return m_scale; }
        // void SetScale(const glm::vec3& scale) { m_scale = scale; }

    private:
        glm::vec3 m_position{};
        // glm::quat m_rotation{};  // might add later
        // glm::vec3 m_scale{1.0f, 1.0f, 1.0f};
    };
}