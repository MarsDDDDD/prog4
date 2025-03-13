#include "Subject.h"

using namespace dae;

void Subject::Notify(const GameObject* actor, Observer::EventId event)
{
	for (const auto& pObserver : m_Observers)
	{
		pObserver->Notify(actor, event);
	}
}
