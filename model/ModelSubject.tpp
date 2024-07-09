#include "ModelSubject.h"

#include <iostream>
#include <typeinfo>

template<typename T, class event_class>
ModelSubject<T, event_class>::ModelSubject()
{
    m_superClass = static_cast<T *>(this);
    m_isNotifying = false;
}

template <typename T, class event_class>
void ModelSubject<T, event_class>::subscribe(ModelObserver<T,event_class> *observer, event_class event_type)
{
    if(m_isNotifying)
    {
        std::cerr << "(" << typeid(m_superClass).name() << ") ModelSubject::subscribe -->> Unable to subscribe. A notification is in progress!" << std::endl;
        return;
    }

    if(observer != nullptr)
    {
        auto it = m_observers.find(event_type);
        if (it == m_observers.end())
            m_observers[event_type] = ObserverList();

        m_observers[event_type].push_back(observer);
    }
}

template <typename T, class event_class>
void ModelSubject<T, event_class>::unsubscribe(ModelObserver<T,event_class> *observer, event_class event_type)
{
    if(m_isNotifying)
    {
        std::cerr << "(" << typeid(m_superClass).name() << ") ModelSubject::unsubscribe -->> Unable to unsubscribe. A notification is in progress!" << std::endl;
        return;
    }

    auto it = m_observers.find(event_type);
    if (it != m_observers.end())
    {
        ObserverList &observer_list = m_observers[event_type];
        unsigned short count = 0;
        for(auto obs : observer_list)
        {
            short id1 = obs->getObserverID();
            short id2 = observer->getObserverID();
            if(id1 == id2)
                observer_list.erase(observer_list.begin() + count);

            count++;
        }
    }
}

template <typename T, class event_class>
void ModelSubject<T, event_class>::notify(event_class event_type, ModelObserver<T, event_class> *caller)
{
    if(m_isNotifying)
    {
        std::cerr << "(" << typeid(m_superClass).name() << ") ModelSubject::notify -->> A previous notification is in progress!" << std::endl;
//        return;
    }

    m_isNotifying = true;

    auto it = m_observers.find(event_type);
    if (it != m_observers.end())
    {
        ObserverList &observer_list = m_observers[event_type];
        for(auto &observer : observer_list)
            if(observer != caller)
            {
                observer->update(m_superClass, event_type);
            }
    }

    m_isNotifying = false;
}

template <typename T, class event_class>
void ModelSubject<T, event_class>::notify(ModelObserver<T, event_class> *caller)
{
    if(m_isNotifying)
    {
        std::cerr << "(" << typeid(m_superClass).name() << ") ModelSubject::notify -->> A previous notification is in progress!" << std::endl;
    }

    if(m_eventList.size() == 0)
        return;

    m_isNotifying = true;

    std::map<ModelObserver<T, event_class>*, std::vector<event_class> > notification_list;

    for(auto &event : m_eventList)
    {
        auto it = m_observers.find(event);
        if (it != m_observers.end())
        {
            ObserverList &observer_list = m_observers[event];
            for(auto &observer : observer_list)
                if(observer != caller)
                    notification_list[observer].push_back(event);
        }
    }

    for(auto &data : notification_list)
        for(auto &event : data.second)
            data.first->update(m_superClass, event);

    m_eventList.clear();

    m_isNotifying = false;
}

template<typename T, class event_class>
bool ModelSubject<T, event_class>::hasNotifications() const
{
    return (m_eventList.size() > 0);
}

template <typename T, class event_class>
bool ModelSubject<T, event_class>::isNotifying() const
{
    return m_isNotifying;
}

template<typename T, class event_class>
void ModelSubject<T, event_class>::addEvent(event_class event)
{
    for(auto &e: m_eventList)
        if(e == event)
            return;

    m_eventList.push_back(event);
}
