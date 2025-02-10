#pragma once

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void Update(float deltaTime) = 0; // Pure virtual, must be implemented by derived classes
        GameObject* GetGameObject() const { return m_gameObject; }
        void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }

    protected:
        Component() = default;

    private:
        GameObject* m_gameObject = nullptr;
    };
}
