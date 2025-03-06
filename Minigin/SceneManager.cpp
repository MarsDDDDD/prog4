#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime) // Modified Update method implementation
{
	for (auto& scene : m_pScenes)
	{
		scene->Update(deltaTime); // Pass deltaTime to Scene::Update
	}
}

void dae::SceneManager::FixedUpdate(float fixedTimeStep) // New FixedUpdate method implementation
{
	for (auto& scene : m_pScenes)
	{
		scene->FixedUpdate(fixedTimeStep); // Pass fixedTimeStep to Scene::FixedUpdate
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_pScenes.emplace_back(scene);
	return *scene;
}