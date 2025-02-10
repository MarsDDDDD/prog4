#include "SceneManager.h"
#include "Scene.h"

namespace dae {

    void SceneManager::Update(float deltaTime) // Modified to take deltaTime
    {
        for (auto& scene : m_scenes)
        {
            scene->Update(deltaTime); // Pass deltaTime to Scene Update
        }
    }

    void SceneManager::Render()
    {
        for (const auto& scene : m_scenes)
        {
            scene->Render();
        }
    }

    Scene& SceneManager::CreateScene(const std::string& name)
    {
        const auto& scene = std::shared_ptr<Scene>(new Scene(name));
        m_scenes.push_back(scene);
        return *scene;
    }
}