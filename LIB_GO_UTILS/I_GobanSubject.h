#ifndef IGOBANSUBJECT_H
#define IGOBANSUBJECT_H

#include<unordered_set>
#include"I_GobanObserver.h"

class I_GobanSubject{
public:
    virtual ~I_GobanSubject(){}

    virtual void NotifyObservers(const sf::Vector2i& pos, int infl_value)
    {
         std::unordered_set<I_GobanObserver*>::iterator it;
         for(it = m_observers.begin(); it != m_observers.end(); ++it )
         {
            (*it)->Notify(pos, infl_value);
         }
    }

    void RegisterObserver(I_GobanObserver* obs)
    {
        m_observers.insert(obs);
    }

    void UnregisterObserver(I_GobanObserver* obs)
    {
        m_observers.erase(obs);
    }

protected:
    std::unordered_set<I_GobanObserver*> m_observers;
};
#endif // IGOBANSUBJECT_H

