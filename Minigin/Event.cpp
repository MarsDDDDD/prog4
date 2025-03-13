#include "Event.h"

using namespace dae;

void Event::Notify(const GameObject* actor, Observer::EventId event)
{
	for (const auto& pObserver : m_pObservers)
	{
		pObserver->Notify(actor, event);
	}
}
