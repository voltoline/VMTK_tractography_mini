#ifndef VMTK_BASE_TOOLTAB_SUBJECT_H
#define VMTK_BASE_TOOLTAB_SUBJECT_H

//#include <vector>
//#include <iostream>
//#include <ui/components/vmtkBaseToolTabObserver.h>

//template <typename T>
//class vmtkBaseToolTabSubject
//{
//public:
//    vmtkBaseToolTabSubject();
//    virtual ~vmtkBaseToolTabSubject() { }

//    virtual void attach(vmtkBaseToolTabObserver<T> *_new_observer);
//    virtual void detach(vmtkBaseToolTabObserver<T> *_observer);
//    virtual void newList();
//    virtual void deleteList(unsigned short _list_pos);
//    virtual void changeListPosition(unsigned short _list_pos);
//    unsigned short getActualListPos();

//protected:
//    typedef struct _observer
//    {
//        vmtkBaseToolTabObserver<T> *m_observer_ptr;
//        unsigned short m_id;
//    }Observer;

//    const std::vector<Observer> *getObserverList();
//    virtual void eventObserverAttached() { }
//    virtual void eventObserverDetached() { }
//    virtual void eventListCreated() { }
//    virtual void eventListDeleted() { }
//    virtual void eventListPositionChanged() { }

//private:

//    unsigned short newId();

//    std::vector<std::vector<Observer> > m_lists;
//    unsigned short m_actual_list;
//    unsigned short m_id_counter;
//};

//template <typename T>
//unsigned short vmtkBaseToolTabSubject<T>::newId()
//{
//    unsigned short id = m_id_counter;
//    m_id_counter++;
//    return id;
//}

//template <typename T>
//vmtkBaseToolTabSubject<T>::vmtkBaseToolTabSubject()
//{
//    m_actual_list = 0;
//    m_id_counter = 0;
//}

//template <typename T>
//void vmtkBaseToolTabSubject<T>::attach(vmtkBaseToolTabObserver<T> *_new_observer)
//{
//    if(m_lists.size() > 0)
//    {
//        Observer new_observer;
//        new_observer.m_observer_ptr = _new_observer;
//        new_observer.m_id = newId();
//        m_lists[m_actual_list].push_back(new_observer);
//        _new_observer->setSubject(dynamic_cast<T*>(this));
//        _new_observer->setObserverId(new_observer.m_id);

//        eventObserverAttached();

//    } else {

//        std::cerr << "vmtkBaseToolTabSubject::attach -> List of observers not created!" << std::endl;
//    }
//}

//template <typename T>
//void vmtkBaseToolTabSubject<T>::detach(vmtkBaseToolTabObserver<T> *_observer)
//{
//    if(m_lists.size() > 0)
//    {
//        if(_observer->getSubject() != NULL)
//        {
//            unsigned short _observer_id = _observer->getObserverId();
//            for(unsigned short i = 0; i < m_lists.size(); i++)
//                for(unsigned short j = 0; j < m_lists[i].size(); j++)
//                {
//                    if(m_lists[i][j].m_id == _observer_id)
//                    {
//                        m_lists[i].erase(m_lists[i].begin() + j);
//                        _observer->setSubject(NULL);
//                        _observer->setObserverId(0);

//                        eventObserverDetached();

//                        return;
//                    }
//                }
//        }

//    } else {

//        std::cerr << "vmtkBaseToolTabSubject::detach -> List of observers not created!" << std::endl;
//    }
//}

//template <typename T>
//void vmtkBaseToolTabSubject<T>::newList()
//{
//    m_lists.push_back(std::vector<Observer>());
//    m_actual_list = m_lists.size() - 1;

//    eventListCreated();
//}

//template <typename T>
//void vmtkBaseToolTabSubject<T>::deleteList(unsigned short _list_pos)
//{
//    if(m_lists.size() > _list_pos)
//    {
//        m_lists.erase(m_lists.begin() + _list_pos);

//        eventListDeleted();

//    } else {

//        std::cerr << "vmtkBaseToolTabSubject::deleteList -> List position out of range!" << std::endl;
//    }
//}

//template <typename T>
//void vmtkBaseToolTabSubject<T>::changeListPosition(unsigned short _list_pos)
//{
//    if(m_lists.size() > _list_pos)
//    {
//        m_actual_list = _list_pos;

//        eventListPositionChanged();

//    } else {
//        std::cerr << "vmtkBaseToolTabSubject::changeList -> List position out of range!" << std::endl;

//    }
//}

//template <typename T>
//unsigned short vmtkBaseToolTabSubject<T>::getActualListPos()
//{
//    return m_actual_list;
//}

//template <typename T>
//const std::vector<typename vmtkBaseToolTabSubject<T>::Observer>* vmtkBaseToolTabSubject<T>::getObserverList()
//{
//    if(m_lists.size() > 0)
//    {
//        return &m_lists[m_actual_list];

//    } else {

//        std::cerr << "vmtkBaseToolTabSubject::getObserverList -> List of observers not created!" << std::endl;
//    }

//    return NULL;
//}

#endif // VMTK_BASE_TOOLTAB_SUBJECT_H
