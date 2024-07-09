#ifndef EVENT_NOTIFIER_H
#define EVENT_NOTIFIER_H

#include <vector>
#include <map>
#include "Signal.hpp"
#include <iostream>

template <class event_class, typename... Args>
class EventNotifier
{
public:
    EventNotifier()
    {
        m_isNotifying = false;
    }

    virtual ~EventNotifier() { }

    virtual int subscribe(event_class event, std::function<void(Args...)> slot )
    {
        if(m_isNotifying)
        {
            std::cerr << "EventNotifier::subscribe -->> A notification is in progress!" << std::endl;
        }

        auto it = m_signals.find(event);
        if (it == m_signals.end())
            m_signals[event] = Signal<Args...>();

        return m_signals[event].connect(slot);
    }

    virtual void unsubscribe(event_class event, int slot_id)
    {
        if(m_isNotifying)
        {
            std::cerr << "EventNotifier::unsubscribe -->> A notification is in progress!" << std::endl;
        }

        auto it = m_signals.find(event);
        if (it != m_signals.end())
            m_signals[event].disconnect(slot_id);
    }

    virtual void notifyAll(event_class event, Args... args)
    {
        if(m_isNotifying)
        {
            std::cerr << "EventNotifier::notifyAll -->> A previous notification is in progress!" << std::endl;
        }

        m_isNotifying = true;

        auto it = m_signals.find(event);
        if (it != m_signals.end())
            m_signals[event].emitAllSignals(args...);

        m_isNotifying = false;
    }

    virtual void notify(event_class event, int slot_id, Args... args)
    {
        if(m_isNotifying)
        {
            std::cerr << "EventNotifier::notify -->> A previous notification is in progress!" << std::endl;
        }

        m_isNotifying = true;

        auto it = m_signals.find(event);
        if (it != m_signals.end())
            m_signals[event].emitSignals(slot_id, args...);

        m_isNotifying = false;
    }

    bool isNotifying() const
    {
        return m_isNotifying;
    }

protected:
    typedef std::map<event_class, Signal<Args...> > SignalMap;

    SignalMap& getSignals()
    {
        return m_signals;
    }

private:
    SignalMap m_signals;
    bool m_isNotifying;
};

#endif // EVENT_NOTIFIER_H
