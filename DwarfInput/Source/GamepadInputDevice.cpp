#include "DwarfInput/GamepadInputDevice.h"
#include "DwarfInput/InputSettings.h"

#include <DwarvenCore/Math/Math.h>

#include <GLFW/glfw3.h>

df::GamepadInputDevice::GamepadInputDevice(const InputSettings& inputSettings, int gamepadId)
	: m_InputSettings(inputSettings)
	, m_GamepadId(gamepadId) {

	for (auto& state : m_ButtonStates) {
		state = 0;
	}

	for (auto& state : m_AxisStates) {
		state = 0.0f;
	}

	m_CursorX = 0.0;
	m_CursorY = 0.0;

	m_RedirectionSettings.Setup(glfwGetJoystickName(gamepadId));
}

auto df::GamepadInputDevice::GetCursorX() const -> double {
	return m_CursorX;
}

auto df::GamepadInputDevice::GetCursorY() const -> double {
	return m_CursorY;
}

auto df::GamepadInputDevice::GetAxis(int axis) const -> double {
	const AxisMapping& axisMapping = m_InputSettings.GetAxisMapping(axis);
	for (const auto& inputMapping : axisMapping.m_InputMappings) {
		switch (inputMapping.m_Type) {
		case EInpitAxisType::GamepadButton:
			return IsPressed(inputMapping.m_GamepadButton) ? inputMapping.m_Scale : 0.0;
		case EInpitAxisType::GamepadAxis:
			return GetAxis(inputMapping.m_GamepadAxis) * inputMapping.m_Scale;
		default:
			break;
		}
	}

	return 0.0;
}

void df::GamepadInputDevice::Update() {
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(m_GamepadId, &count);
	for (int i = 0; i < count; ++i) {
		if (m_ButtonStates[i] != buttons[i]) {
			m_ButtonStates[i] = buttons[i];

			for (int b = 0; b < int(EGamepadButton::NumButtons); ++b) {
				if (i == m_RedirectionSettings.m_Buttons[b]) {
					ProcessButton(EGamepadButton(b), m_ButtonStates[i]);
				}
			}
		}
	}

	const float* axes = glfwGetJoystickAxes(m_GamepadId, &count);
	for (int i = 0; i < count; ++i) {
		m_AxisStates[i] = axes[i];
	}

	m_CursorX += GetAxis(EGamepadAxis::LX) * 10.0;
	m_CursorY -= GetAxis(EGamepadAxis::LY) * 10.0;
}

auto df::GamepadInputDevice::GetAxis(EGamepadAxis axis) const -> float {
	const auto& value = m_AxisStates[m_RedirectionSettings.m_AxisSettings[int(axis)].Idx];
	const auto& boundA = m_RedirectionSettings.m_AxisSettings[int(axis)].BoundMin.Value;
	const auto& boundB = m_RedirectionSettings.m_AxisSettings[int(axis)].BoundMax.Value;

	return CalibrateAxisValue(RemapAxisValue(value, boundA, boundB));
}

auto df::GamepadInputDevice::GetIndex() const->int {
	return m_GamepadId;
}

bool df::GamepadInputDevice::IsPressed(EGamepadButton gamepadButton) const {
	return m_GamepadButtonStates[int(gamepadButton)];
}

void df::GamepadInputDevice::ProcessButton(EGamepadButton button, bool press) {
	m_GamepadButtonStates[int(button)] = press;
	m_GamepadButtonEatenStates[int(button)] = false;

	if (press) {
		for (const auto& e : m_InputSettings.GetGamepadButtonPressedEvents()[int(button)].m_Events) {
			PushEvent(e);
		}
	} else {
		for (const auto& e : m_InputSettings.GetGamepadButtonReleasedEvents()[int(button)].m_Events) {
			PushEvent(e);
		}
	}
}

auto df::GamepadInputDevice::RemapAxisValue(float value, float boundA, float boundB) const -> float {
	const float uniformValue = (value - boundA) / (boundB - boundA);
	return uniformValue * 2.0f - 1.0f;
}

auto df::GamepadInputDevice::CalibrateAxisValue(float value) const -> float {
	if (value < 0.25f) {
		return Clamp((value + 0.25f) / (1.0f - 0.25f) * 1.05f, -1.0f, 0.0f);
	} else if (value > 0.25f) {
		return Clamp((value - 0.25f) / (1.0f - 0.25f) * 1.05f, 0.0f, 1.0f);
	}

	return 0.0f;
}