#pragma once
#include <vector>
#include <memory>
#include "Singleton.h"
#include "Observer.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	class Observer;

	class Event final : public Singleton<Event>, public Subject
	{
	public:
		void Notify(const GameObject* actor, Observer::EventId event);
	};
}

