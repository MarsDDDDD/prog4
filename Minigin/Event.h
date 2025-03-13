#pragma once
#include <vector>
#include <memory>
#include "Singleton.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class Observer;

	class Event final : public Singleton<Event>
	{
	public:
		template <class T>
		void AddObserver();
		template <class T>
		void RemoveObserver();
		void Notify(const GameObject* actor, Observer::EventId event);

	private:
		std::vector<std::unique_ptr<Observer>> m_pObservers{};
	};


	template<class T>
	inline void Event::AddObserver()
	{
		m_pObservers.push_back(std::make_unique<T>());
	}

	// remove observer
	template<class T>
	inline void Event::RemoveObserver()
	{
		for (size_t i{}; i < m_pObservers.size(); ++i)
		{
			if (typeid(*m_pObservers[i]) == typeid(T))
			{
				m_pObservers.erase(m_pObservers.begin() + i);
				return;
			}
		}
	}

}

