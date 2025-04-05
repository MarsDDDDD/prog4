#include "Subject.h"
#include "GameObject.h"

namespace dae
{
    Subject::~Subject()
    {
        m_pObservers.clear();
    }
    
    void Subject::AddObserver(Observer* observer)
    {
        m_pObservers.push_back(observer);
    }
    
    void Subject::RemoveObserver(Observer* observer)
    {
        m_pObservers.erase(
            std::remove(m_pObservers.begin(), m_pObservers.end(), observer),
            m_pObservers.end());
    }
    
    void Subject::Notify(const GameObject* actor, Observer::EventId event)
    {
        for (const auto& observer : m_pObservers)
        {
            observer->Notify(actor, event);
        }
    }
}
