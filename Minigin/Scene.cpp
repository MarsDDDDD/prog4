#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime) // Modified Update method implementation
{
	for (auto& object : m_objects)
	{
		object->Update(deltaTime); // Pass deltaTime to GameObject::Update
	}
}

void Scene::FixedUpdate(float fixedTimeStep) // New FixedUpdate method implementation
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixedTimeStep); // Pass fixedTimeStep to GameObject::FixedUpdate (to be created)
	}
}


void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}