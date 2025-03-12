#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

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
	//Mark all objects for removal.  This is safer than clearing m_objects directly
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
	for (const auto& object : m_objectsToRemove)
	{
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[object](const std::unique_ptr<GameObject>& obj) { return obj.get() == object; }), m_objects.end());
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