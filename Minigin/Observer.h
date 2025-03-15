#pragma once

namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		enum class EventId
		{
			ACTOR_DIED,
			HEALTH_UPDATED,
			SCORE_UPDATED
		};

		Observer() = default;
		virtual ~Observer() = default;
		Observer(const Observer& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(Observer&& other) = delete;

		virtual void Notify(const GameObject* actor, EventId event) = 0;
	};
}