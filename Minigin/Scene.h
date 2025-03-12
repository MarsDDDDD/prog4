#pragma once
#include "SceneManager.h"

namespace dae
{
	class FPSComponent;
	class GameObject;
    class Scene final
    {
        friend Scene& SceneManager::CreateScene(const std::string& name);
    public:
        void Add(std::unique_ptr<GameObject> object);
        void Remove(std::unique_ptr<GameObject> object);
        void RemoveAll();

        GameObject* CreateGameObject();

        void Update(float deltaTime); // Modified Update method
        void FixedUpdate(float fixedTimeStep); // New FixedUpdate method
        void Render() const;

        void UpdateCleanup();

        ~Scene();
        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) = delete;

    private:
        explicit Scene(const std::string& name);

        std::string m_name;
        std::vector<std::unique_ptr<GameObject>> m_objects{};

        static unsigned int m_idCounter;

        std::vector<std::unique_ptr<GameObject>> m_objectsToRemove; // Changed to unique_ptr
    };
}