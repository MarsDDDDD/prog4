#pragma once
#include <vector>
#include "Observer.h"

namespace dae
{
    class Subject
    {
    public:
        Subject() = default;
        virtual ~Subject();
        
        void AddObserver(Observer* observer);
        void RemoveObserver(Observer* observer);
        
    protected:
        void Notify(const GameObject* actor, Observer::EventId event);
        
    private:
        std::vector<Observer*> m_pObservers{};
    };
}
