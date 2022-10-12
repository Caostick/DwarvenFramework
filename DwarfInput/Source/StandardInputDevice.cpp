#include "DwarfInput/StandardInputDevice.h"
#include "DwarfInput/InputSettings.h"

df::StandardInputDevice::StandardInputDevice(const InputSettings& inputSettings) 
	: m_InputSettings(inputSettings) {
}

void df::StandardInputDevice::Update() {
	m_MouseXPrev = m_MouseX;
	m_MouseYPrev = m_MouseY;
}

auto df::StandardInputDevice::GetCursorX() const -> double {
	return m_MouseX;
}

auto df::StandardInputDevice::GetCursorY() const -> double {
	return m_MouseY;
}

auto df::StandardInputDevice::GetAxis(int axis) const -> double {
	double value = 0.0;

	const AxisMapping& axisMapping = m_InputSettings.GetAxisMapping(axis);
	for (const auto& inputMapping : axisMapping.m_InputMappings) {
		switch (inputMapping.m_Type) {
		case EInpitAxisType::MouseSpeedX:
			value += GetMouseSpeedX() * inputMapping.m_Scale;
			break;
		case EInpitAxisType::MouseSpeedY:
			value += GetMouseSpeedY() * inputMapping.m_Scale;
			break;
		case EInpitAxisType::MouseButton:
			value += IsPressed(inputMapping.m_MouseKey) ? inputMapping.m_Scale : 0.0;
			break;
		case EInpitAxisType::Key:
			value += IsPressed(inputMapping.m_Key) ? inputMapping.m_Scale : 0.0;
			break;
		default:
			break;
		}
	}

	return value;
}

void df::StandardInputDevice::ProcessChar(char /*charCode*/) {

}

void df::StandardInputDevice::ProcessKeyPress(InputKey key, bool repeat) {
	SetKeyEaten(key, false);

	if (int(key) >= 0 && int(key) < int(InputKey::NUM_KEYS)) {
		if (!repeat) {
			SetKeyState(key, true);

			for (const auto& e : m_InputSettings.GetKeyPressedEvents()[int(key)].m_Events) {
				PushEvent(e);
			}
		}
	}
}

void df::StandardInputDevice::ProcessKeyRelease(InputKey key) {
	SetKeyEaten(key, false);

	if (int(key) >= 0 && int(key) < int(InputKey::NUM_KEYS)) {
		SetKeyState(key, false);

		for (const auto& e : m_InputSettings.GetKeyReleasedEvents()[int(key)].m_Events) {
			PushEvent(e);
		}
	}
}

void df::StandardInputDevice::ProcessMousePress(InputMouseKey key) {
	SetMouseKeyEaten(key, false);

	if (int(key) >= 0 && int(key) < int(InputMouseKey::NUM_MOUSE_BUTTONS)) {
		SetMouseKeyState(key, true);

		for (const auto& e : m_InputSettings.GetMousePressedEvents()[int(key)].m_Events) {
			PushEvent(e);
		}
	}
}

void df::StandardInputDevice::ProcessMouseRelease(InputMouseKey key) {
	SetMouseKeyEaten(key, false);

	if (int(key) >= 0 && int(key) < int(InputMouseKey::NUM_MOUSE_BUTTONS)) {
		SetMouseKeyState(key, false);

		for (const auto& e : m_InputSettings.GetMouseReleasedEvents()[int(key)].m_Events) {
			PushEvent(e);
		}
	}
}

void df::StandardInputDevice::ProcessMouseMove(double x, double y) {
	m_MouseX = x;
	m_MouseY = y;
}

void df::StandardInputDevice::ProcessScroll(int value) {
	if (value > 0) {
		SetScrollUpEaten(false);
		for (const auto& e : m_InputSettings.GetScrollUpEvents().m_Events) {
			PushEvent(e);
		}
	} else if (value < 0) {
		SetScrollDownEaten(false);
		for (const auto& e : m_InputSettings.GetScrollDownEvents().m_Events) {
			PushEvent(e);
		}
	}
}

void df::StandardInputDevice::SetKeyState(InputKey key, bool state) {
	m_KeyStates[int(key)] = state;
}

void df::StandardInputDevice::SetMouseKeyState(InputMouseKey key, bool state) {
	m_MouseKeyStates[int(key)] = state;
}

void df::StandardInputDevice::SetKeyEaten(InputKey key, bool eaten) {
	m_KeyEatenStates[int(key)] = eaten;
}

void df::StandardInputDevice::SetMouseKeyEaten(InputMouseKey key, bool eaten) {
	m_MouseKeyEatenStates[int(key)] = eaten;
}

void df::StandardInputDevice::SetScrollUpEaten(bool eaten) {
	m_ScrollUpIsEaten = eaten;
}

void df::StandardInputDevice::SetScrollDownEaten(bool eaten) {
	m_ScrollDownIsEaten = eaten;
}

bool df::StandardInputDevice::IsPressed(InputKey key) const {
	return m_KeyStates[int(key)];
}

bool df::StandardInputDevice::IsPressed(InputMouseKey mouseKey) const {
	return m_MouseKeyStates[int(mouseKey)];
}

auto df::StandardInputDevice::GetMouseSpeedX() const -> double {
	return m_MouseX - m_MouseXPrev;
}

auto df::StandardInputDevice::GetMouseSpeedY() const -> double {
	return m_MouseY - m_MouseYPrev;
}