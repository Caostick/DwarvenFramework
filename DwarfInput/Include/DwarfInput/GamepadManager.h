#pragma once

#include <DwarvenCore/Signal.h>

namespace df {
    class GamepadManager {
    public:
        static void Init();
        static void Release();
        static void Update();

        static void ReconnectGamepads();

        // gamepad index, connected/disconnected
        static Signal<int, bool> OnGamepadConnectionChangedSignal;

    private:
        static void OnGamepadConnectionChanged(int index, int connection);
    };
}