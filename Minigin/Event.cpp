#include "Event.h"

using namespace dae;

void dae::Event::AddObserver(std::unique_ptr<Observer> pObserver)
{
	m_pObservers.push_back(std::move(pObserver));
}

void Event::Notify(const GameObject* actor, Observer::EventId event)
{
	for (const auto& pObserver : m_pObservers)
	{
		pObserver->Notify(actor, event);
	}
}
