#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

GameObject* dae::Scene::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>() };
	return nullptr;
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	m_objectsToRemove.emplace_back(object); // Defer removal
}

void Scene::RemoveAll()
{
	//Mark all objects for removal. This is safer than clearing m_objects directly
	for (const auto& object : m_objects)
	{
		Remove(object.get());
	}
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	// Process deferred removals *after* updating
	for (auto* object : m_objectsToRemove)
	{
		auto it = std::find_if(m_objects.begin(), m_objects.end(),
			[object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; });
		
		if (it != m_objects.end())
		{
			m_objects.erase(it);
		}
	}
	m_objectsToRemove.clear();
}

void Scene::FixedUpdate(float fixedTimeStep)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedTimeStep);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

// Add the implementation for the new method

void Scene::AddObserver(std::shared_ptr<Observer> observer)
{
    m_Observers.push_back(observer);
}