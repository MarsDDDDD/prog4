#pragma once
#include "SceneManager.h"
#include <vector>
#include <string>
#include <memory>

namespace dae
{
	class FPSComponent;
	class GameObject;
	class Observer;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* CreateGameObject();
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

		void Update(float deltaTime); // Modified Update method
		void FixedUpdate(float fixedTimeStep); // New FixedUpdate method
		void Render() const;

		void AddObserver(std::shared_ptr<Observer> observer);

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

		std::vector<GameObject*> m_objectsToRemove;
		std::vector<std::shared_ptr<Observer>> m_Observers;
	};

}