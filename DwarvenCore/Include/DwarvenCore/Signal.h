#pragma once

#include <functional>
#include <vector>

using SlotId = int;

template <typename... Args>
class Signal {
public:
    Signal();

	auto Connect(const std::function<void(Args...)>& func)->SlotId;

    template <typename T>
    auto Connect(T* inst, void (T::* func)(Args...))->SlotId;

    void Disconnect(SlotId slotId);
    void DisconnectAll();
    void Emit(Args... args);

private:
    template <typename... Args>
    struct Slot {
        Slot(SlotId id, std::function<void(Args...)> const& func);

        SlotId                          Id;
        std::function<void(Args...)>    Func;
    };

    std::vector<Slot<Args...>> m_Slots;
    SlotId m_NextId;
};

#include <DwarvenCore/Signal.inl>