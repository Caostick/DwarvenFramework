#pragma once

#include "DwarfInput/Keymaps.h"

namespace df {
    constexpr int GamepadButtonCount = 64;
    constexpr int GamepadInvalidButtonIndex = (GamepadButtonCount - 1);
}

namespace df {
    struct GamepadAxisSettings {
        int Idx;

        union {
            float Left;
            float Bottom;
            float Released;
            float Value;
        } BoundMin;

		union {
			float Right;
			float Top;
			float Pressed;
            float Value;
		} BoundMax;
    };

    struct GamepadRedirectionSettings {
        GamepadRedirectionSettings();

        void Setup(const char* gamepadName);

        // @TODO: Make data-driven instead
        void SetupXBox();
        void SetupXBox360();
        void SetupDualshock4();

        int m_Buttons[int(EGamepadButton::NumButtons)];
        GamepadAxisSettings m_AxisSettings[int(EGamepadAxis::NumAxises)];
    };
}