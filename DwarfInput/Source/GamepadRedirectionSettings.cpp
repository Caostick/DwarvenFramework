#include "DwarfInput/GamepadRedirectionSettings.h"

#include <GLFW/glfw3.h>

#include <string.h>

df::GamepadRedirectionSettings::GamepadRedirectionSettings() {
    for (int& button : m_Buttons) {
        button = GamepadInvalidButtonIndex;
    }

    for (auto& axisLimits : m_AxisSettings) {
        axisLimits.BoundMin.Left = -1.0f;
        axisLimits.BoundMax.Right = 1.0f;
    }
}

void df::GamepadRedirectionSettings::Setup(const char* gamepadName) {
	// @TODO: Make it data-driven and check "Xbox Controller"
    if (!strcmp(gamepadName, "Xbox 360 Controller")) {
        SetupXBox360();
    } else if (!strcmp(gamepadName, "Wireless Controller")) {
        SetupDualshock4();
	} else {
		SetupXBox();
	}
}

void df::GamepadRedirectionSettings::SetupXBox() {
	m_Buttons[int(EGamepadButton::A)] = 1;
	m_Buttons[int(EGamepadButton::B)] = 2;
	m_Buttons[int(EGamepadButton::X)] = 0;
	m_Buttons[int(EGamepadButton::Y)] = 3;
	m_Buttons[int(EGamepadButton::LB)] = 4;
	m_Buttons[int(EGamepadButton::RB)] = 5;
	m_Buttons[int(EGamepadButton::LT)] = 6;
	m_Buttons[int(EGamepadButton::RT)] = 7;
	m_Buttons[int(EGamepadButton::Back)] = 9;
	m_Buttons[int(EGamepadButton::Start)] = 8;
	m_Buttons[int(EGamepadButton::LS)] = 10;
	m_Buttons[int(EGamepadButton::RS)] = 11;
	m_Buttons[int(EGamepadButton::Up)] = 14;
	m_Buttons[int(EGamepadButton::Right)] = 15;
	m_Buttons[int(EGamepadButton::Down)] = 16;
	m_Buttons[int(EGamepadButton::Left)] = 17;


	m_AxisSettings[int(EGamepadAxis::LX)].Idx = 0;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::LY)].Idx = 1;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMin.Bottom = 1.0f;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMax.Top = -1.0f;

	m_AxisSettings[int(EGamepadAxis::RX)].Idx = 2;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RY)].Idx = 3;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMin.Bottom = 1.0f;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMax.Top = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LT)].Idx = 4;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMax.Pressed = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RT)].Idx = 5;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMax.Pressed = 1.0f;
}

void df::GamepadRedirectionSettings::SetupXBox360() {
	m_Buttons[int(EGamepadButton::A)] = 0;
	m_Buttons[int(EGamepadButton::B)] = 1;
	m_Buttons[int(EGamepadButton::X)] = 2;
	m_Buttons[int(EGamepadButton::Y)] = 3;
	m_Buttons[int(EGamepadButton::LB)] = 4;
	m_Buttons[int(EGamepadButton::RB)] = 5;
	m_Buttons[int(EGamepadButton::Back)] = 6;
	m_Buttons[int(EGamepadButton::Start)] = 7;
	m_Buttons[int(EGamepadButton::LS)] = 8;
	m_Buttons[int(EGamepadButton::RS)] = 9;
	m_Buttons[int(EGamepadButton::Up)] = 10;
	m_Buttons[int(EGamepadButton::Right)] = 11;
	m_Buttons[int(EGamepadButton::Down)] = 12;
	m_Buttons[int(EGamepadButton::Left)] = 13;


	m_AxisSettings[int(EGamepadAxis::LX)].Idx = 0;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::LY)].Idx = 1;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMin.Bottom = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMax.Top = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RX)].Idx = 2;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RY)].Idx = 3;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMin.Bottom = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMax.Top = 1.0f;

	m_AxisSettings[int(EGamepadAxis::LT)].Idx = 4;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMax.Pressed = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RT)].Idx = 5;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMax.Pressed = 1.0f;
}

void df::GamepadRedirectionSettings::SetupDualshock4() {
	m_Buttons[int(EGamepadButton::A)] = 1;
	m_Buttons[int(EGamepadButton::B)] = 2;
	m_Buttons[int(EGamepadButton::X)] = 0;
	m_Buttons[int(EGamepadButton::Y)] = 3;
	m_Buttons[int(EGamepadButton::LB)] = 4;
	m_Buttons[int(EGamepadButton::RB)] = 5;
    m_Buttons[int(EGamepadButton::LT)] = 6;
    m_Buttons[int(EGamepadButton::RT)] = 7;
	m_Buttons[int(EGamepadButton::Back)] = 9;
	m_Buttons[int(EGamepadButton::Start)] = 8;
	m_Buttons[int(EGamepadButton::LS)] = 10;
	m_Buttons[int(EGamepadButton::RS)] = 11;
	m_Buttons[int(EGamepadButton::Up)] = 14;
	m_Buttons[int(EGamepadButton::Right)] = 15;
	m_Buttons[int(EGamepadButton::Down)] = 16;
	m_Buttons[int(EGamepadButton::Left)] = 17;


	m_AxisSettings[int(EGamepadAxis::LX)].Idx = 0;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::LY)].Idx = 1;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMin.Bottom = 1.0f;
	m_AxisSettings[int(EGamepadAxis::LY)].BoundMax.Top = -1.0f;

	m_AxisSettings[int(EGamepadAxis::RX)].Idx = 2;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMin.Left = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RX)].BoundMax.Right = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RY)].Idx = 3;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMin.Bottom = 1.0f;
	m_AxisSettings[int(EGamepadAxis::RY)].BoundMax.Top = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LT)].Idx = 4;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::LT)].BoundMax.Pressed = 1.0f;

	m_AxisSettings[int(EGamepadAxis::RT)].Idx = 5;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMin.Released = -1.0f;
	m_AxisSettings[int(EGamepadAxis::RT)].BoundMax.Pressed = 1.0f;
}