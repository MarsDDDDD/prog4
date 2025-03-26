#include "Event.h"

using namespace dae;

void Event::Notify(const GameObject* actor, Observer::EventId event)
{
	Subject::Notify(actor, event);
}
