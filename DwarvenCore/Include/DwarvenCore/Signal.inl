#pragma once

template <typename... Args>
Signal<Args...>::Signal()
    : m_NextId(0) {}

template <typename... Args>
auto Signal<Args...>::Connect(std::function<void(Args...)> const& func) -> SlotId {
    const SlotId id = m_NextId++;
    m_Slots.push_back(Slot<Args...>(id, func));
    return id;
}

template <typename... Args>
template <typename T>
auto Signal<Args...>::Connect(T* inst, void (T::* func)(Args...)) -> SlotId {
    return Connect([=](Args... args) {
        (inst->*func)(args...);
        });
}

template <typename... Args>
void Signal<Args...>::Disconnect(SlotId slotId) {
    const size_t numSlots = m_Slots.size();
    for (size_t i = 0; i < numSlots; ++i) {
        if (m_Slots[i].Id == slotId) {
            m_Slots[i] = m_Slots[numSlots - 1];
            m_Slots.pop_back();
            break;
        }
    }
}

template <typename... Args>
void Signal<Args...>::DisconnectAll() {
    m_Slots.clear();
}

template <typename... Args>
void Signal<Args...>::Emit(Args... args) {
    for (Slot<Args...>& slot : m_Slots) {
        slot.Func(args...);
    }
}



template<typename ...Args>
template<typename ...Args2>
inline Signal<Args...>::Slot<Args2...>::Slot(SlotId id, std::function<void(Args2...)> const& func)
    : Id(id)
    , Func(func) {}