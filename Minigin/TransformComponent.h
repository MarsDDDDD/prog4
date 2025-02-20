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

        TransformComponent(const TransformComponent&) = delete;
        TransformComponent(TransformComponent&&) = delete;
        TransformComponent& operator= (const TransformComponent&) = delete;
        TransformComponent& operator= (const TransformComponent&&) = delete;

        // Getters and Setters
        const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
        void SetLocalPosition(const glm::vec3& position) { m_LocalPosition = position; }
        void SetLocalPosition(float x, float y, float z) { m_LocalPosition = glm::vec3(x, y, z); }


        const glm::vec3& GetWorldPosition();


		const void SetPositionDirty() { m_IsDirty = true; }
		bool IsPositionDirty() { return m_IsDirty; }
    private:
        glm::vec3 m_LocalPosition{};
        glm::vec3 m_WorldPosition;
        bool m_IsDirty;

        void UpdateWorldPosition();
    };
}