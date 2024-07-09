#ifndef MODEL_OBSERVER_H
#define MODEL_OBSERVER_H

template <class subject_type, class event_class = unsigned short>
class ModelObserver
{
public:
    ModelObserver()
    {
        mObserverID = mIDCounter;
        mIDCounter++;
    }

    virtual ~ModelObserver() { }

    short getObserverID()
    {
        return mObserverID;
    }

    virtual void update(subject_type *subject, event_class event_type) = 0;

private:
    short mObserverID;
    static short mIDCounter;

};

template <class subject_type, class event_class>
short ModelObserver<subject_type, event_class>::mIDCounter = 0;

#endif // MODEL_OBSERVER_H
