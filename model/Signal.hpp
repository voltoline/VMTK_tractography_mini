#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <map>

// A signal object may call multiple slots with the
// same signature. You can connect functions to the signal
// which will be called when the emit() method on the
// signal object is invoked. Any argument passed to emit()
// will be passed to the given functions.


// Modificado por Iván Rubianes e Raphael Voltoline Ramos segundo o padrão original udado no blog http://simmesimme.github.io/tutorials/2015/09/20/signal-slot //

/*!
 * @brief Create a slot function to a signal
 * @param[in] inst signal
 * @param[in] *func function name
 * @return slot_f slot function reference
 */
template<class T, class ... Args>
std::function<void(Args...)> CREATE_SLOT(T *inst, void (T::*func)(Args...))
{
    auto slot_f = [inst, func](Args... args){ (inst->*func)(args...); };
    return slot_f;
}

template <typename... Args>
/**
 * @brief
 *
 */
class Signal {

public:

    /**
     * @brief
     *
     */
    Signal() : m_slotCountID(0) {}

    // copy creates new signal
    /**
     * @brief
     *
     * @param other
     */
    Signal(Signal const& other) : m_slotCountID(0) {}

    // connects a std::function to the signal. The returned
    // value can be used to disconnect the function again
    /**
     * @brief
     *
     * @param slot
     * @return int
     */
    int connect(std::function<void(Args...)> const& slot) const {
        m_slots.insert(std::make_pair(++m_slotCountID, slot));
        return m_slotCountID;
    }

    // disconnects a previously connected function
    /**
     * @brief
     *
     * @param id
     */
    void disconnect(int id) const {
        m_slots.erase(id);
    }

    // disconnects all previously connected functions
    /**
     * @brief
     *
     */
    void disconnectAll() const {
        m_slots.clear();
    }

    // calls all connected functions
    /**
     * @brief
     *
     * @param p
     */
    void emitAllSignals(Args... p) {
        for(auto it : m_slots) {
            it.second(p...);
        }
    }

    /**
     * @brief
     *
     * @param slot_id
     * @param p
     */
    void emitSignals(int slot_id, Args... p) {
        for(auto it : m_slots) {
            if(it.first != slot_id)
                it.second(p...);
        }
    }

private:

    /**
     * @brief
     *
     * @param Args...
     * @return std::map<int, std::function<void > >
     */
    mutable std::map<int, std::function<void(Args...)>> m_slots;
    mutable int m_slotCountID; /**< TODO: describe */
};

#endif /* SIGNAL_HPP */
