#pragma once
#include "BaseComponent.h"
#include "TransformComponent.h"
#include <glm.hpp>

namespace dae
{
    class OrbitComponent : public BaseComponent
    {
    public:
        OrbitComponent(GameObject* pOwner, float radius, float speed);

        void Update(float deltaTime) override;

    private:
        float m_Radius;
        float m_Speed;
        float m_Angle;
    };

}
