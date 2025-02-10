#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "Component.h"

namespace dae
{
    class Texture2D;

    class GameObject final // Marked as final as per instruction
    {
    public:
        template <typename T>
        std::shared_ptr<T> AddComponent()
        {
            if (HasComponent<T>())
            {
                return GetComponent<T>(); // Or perhaps throw an exception, depending on desired behavior
            }
            std::shared_ptr<T> component = std::make_shared<T>();
            component->SetGameObject(this);
            m_components.push_back(component);
            return component;
        }

        template <typename T>
        std::shared_ptr<T> GetComponent() const
        {
            for (const auto& comp : m_components)
            {
                if (std::shared_ptr<T> derivedComp = std::dynamic_pointer_cast<T>(comp))
                {
                    return derivedComp;
                }
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent() const
        {
            return GetComponent<T>() != nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            for (auto it = m_components.begin(); it != m_components.end(); ++it)
            {
                if (std::dynamic_pointer_cast<T>(*it))
                {
                    m_components.erase(it);
                    return; // Assuming only one component of each type is desired, or remove break for all
                }
            }
        }


        void Update(float deltaTime); // Modified to take deltaTime
        void Render() const;

        void SetTexture(const std::string& filename);
        void SetPosition(float x, float y);
        Transform& GetTransform() { return m_transform; } // Added getter for Transform

        GameObject() = default;
        ~GameObject() = default; // Modified to default
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

    private:
        Transform m_transform{};
        std::shared_ptr<Texture2D> m_texture{};
        std::vector<std::shared_ptr<Component>> m_components; // Vector to hold components
    };
}