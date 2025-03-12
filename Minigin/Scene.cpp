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


void Scene::Remove(std::unique_ptr<GameObject> object)
{
    m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
    m_objects.clear();
}

GameObject* Scene::CreateGameObject()
{
    auto pGameObject = std::make_unique<GameObject>();

    GameObject* pGameObjectToReturn = pGameObject.get();

    Add(std::move(pGameObject));
    return pGameObjectToReturn;
}

void Scene::Update(float deltaTime)
{
    for (auto& object : m_objects)
    {
        object->Update(deltaTime);
    }
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

void Scene::UpdateCleanup()
{
    // Deferred removal
    m_objects.erase(std::remove_if(begin(m_objects), end(m_objects), [](const auto& pGameObject)
        {
            return pGameObject->IsPendingRemoval();
        }), end(m_objects));
}