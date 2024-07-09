#ifndef MODEL_SUBJECT_H
#define MODEL_SUBJECT_H

#include <vector>
#include <map>
#include "ModelObserver.h"

template <typename T, class event_class = unsigned short>
class ModelSubject
{
public:
    ModelSubject();
    virtual ~ModelSubject() { }

    virtual void subscribe(ModelObserver<T,event_class> *observer, event_class event_type);
    virtual void unsubscribe(ModelObserver<T,event_class> *observer, event_class event_type);
    virtual void notify(event_class event_type, ModelObserver<T, event_class> *caller = nullptr);
    virtual void notify(ModelObserver<T, event_class> *caller = nullptr);

    bool hasNotifications() const;
    bool isNotifying() const;

protected:
    virtual void addEvent(event_class event);

private:

    typedef std::vector<ModelObserver<T, event_class>*> ObserverList;
    typedef std::map<event_class, ObserverList> ObserverMap;
    ObserverMap m_observers;
    T *m_superClass;
    bool m_isNotifying;

    std::vector<event_class> m_eventList;
};

#include "ModelSubject.tpp"

#endif // MODEL_SUBJECT_H
