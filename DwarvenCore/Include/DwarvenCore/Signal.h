#pragma once

#include <functional>
#include <vector>

using SlotId = int;

template <typename... Args>
class Signal {
public:
    Signal()
        : m_NextId(0) {}

    SlotId Connect(std::function<void(Args...)> const& func) {
        const SlotId id = m_NextId++;
        m_Slots.push_back(Slot<Args...>(id, func));
        return id;
    }

    template <typename T>
    SlotId Connect(T* inst, void (T::*func)(Args...)) {
        return Connect([=](Args... args) {
            (inst->*func)(args...);
        });
    }

    void Disconnect(SlotId slotId) {
        const size_t numSlots = m_Slots.size();
        for (size_t i = 0; i < numSlots; ++i) {
            if (m_Slots[i].Id == slotId) {
                m_Slots[i] = m_Slots[numSlots - 1];
                m_Slots.pop_back();
                break;
            }
        }
    }

    void DisconnectAll() {
        m_Slots.clear();
    }

    void Emit(Args... args) {
        for (Slot<Args...>& slot : m_Slots) {
            slot.Func(args...);
        }
    }
private:
    template <typename... Args>
    struct Slot {
        Slot(SlotId id, std::function<void(Args...)> const& func)
            : Id(id)
            , Func(func) {}

        SlotId                          Id;
        std::function<void(Args...)>    Func;
    };

    std::vector<Slot<Args...>> m_Slots;
    SlotId m_NextId;
};