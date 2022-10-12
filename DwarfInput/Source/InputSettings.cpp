#include "DwarfInput/InputSettings.h"

#include <DwarvenCore/Assert.h>

void df::InputSettings::Reset() {
	m_KeyPressedEvents.clear();
	m_KeyReleasedEvents.clear();
	m_MousePressedEvents.clear();
	m_MouseReleasedEvents.clear();
	m_ScrollUpEvents.m_Events.clear();
	m_ScrollDownEvents.m_Events.clear();
	m_GamepadButtonPressedEvents.clear();
	m_GamepadButtonReleasedEvents.clear();

	m_ScrollUpEvents.m_Events.clear();
	m_ScrollDownEvents.m_Events.clear();

	m_AxisMappings.clear();


	m_KeyPressedEvents.resize(size_t(InputKey::NUM_KEYS));
	m_KeyReleasedEvents.resize(size_t(InputKey::NUM_KEYS));
	m_MousePressedEvents.resize(size_t(InputMouseKey::NUM_MOUSE_BUTTONS));
	m_MouseReleasedEvents.resize(size_t(InputMouseKey::NUM_MOUSE_BUTTONS));
	m_GamepadButtonPressedEvents.resize(size_t(EGamepadButton::NumButtons));
	m_GamepadButtonReleasedEvents.resize(size_t(EGamepadButton::NumButtons));
}

void df::InputSettings::RegisterAxis(int axisId, double scale, InputKey key) {
	const EInpitAxisType axisMappingType = EInpitAxisType::Key;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType && inputMapping.m_Key == key) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_Key = key;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterAxis(int axisId, double scale, InputMouseKey key) {
	const EInpitAxisType axisMappingType = EInpitAxisType::MouseButton;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType && inputMapping.m_MouseKey == key) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_MouseKey = key;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterAxis(int axisId, double scale, EGamepadButton button) {
	const EInpitAxisType axisMappingType = EInpitAxisType::GamepadButton;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType && inputMapping.m_GamepadButton == button) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_GamepadButton = button;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterAxis(int axisId, double scale, EGamepadAxis axis) {
	const EInpitAxisType axisMappingType = EInpitAxisType::GamepadAxis;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType && inputMapping.m_GamepadAxis == axis) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_GamepadAxis = axis;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterMouseSpeedXAxis(int axisId, double scale) {
	const EInpitAxisType axisMappingType = EInpitAxisType::MouseSpeedX;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterMouseSpeedYAxis(int axisId, double scale) {
	const EInpitAxisType axisMappingType = EInpitAxisType::MouseSpeedY;
	auto& axisMapping = GetAxisMapping(axisId);

	for (auto& inputMapping : axisMapping.m_InputMappings) {
		if (inputMapping.m_Type == axisMappingType) {
			DFAssert(false, "Axis input redefinition!");
			return;
		}
	}

	AxisInputMapping inputMapping;
	inputMapping.m_Type = axisMappingType;
	inputMapping.m_Scale = scale;
	axisMapping.m_InputMappings.emplace_back(inputMapping);
}

void df::InputSettings::RegisterGamepadButtonEvent(EGamepadButton button, bool press, int eventId) {
	if (int(button) < 0 || int(button) >= int(EGamepadButton::NumButtons)) {
		return;
	}

	if (press) {
		m_GamepadButtonPressedEvents[int(button)].m_Events.push_back(eventId);
	} else {
		m_GamepadButtonReleasedEvents[int(button)].m_Events.push_back(eventId);
	}
}

void df::InputSettings::RegisterInputEvent(InputKey key, bool press, int eventId) {
	if (int(key) < 0 || int(key) >= int(InputKey::NUM_KEYS)) {
		return;
	}

	if (press) {
		m_KeyPressedEvents[int(key)].m_Events.push_back(eventId);
	} else {
		m_KeyReleasedEvents[int(key)].m_Events.push_back(eventId);
	}
}

void df::InputSettings::RegisterInputMouseEvent(InputMouseKey key, bool press, int eventId) {
	if (int(key) < 0 || int(key) >= int(InputMouseKey::NUM_MOUSE_BUTTONS)) {
		return;
	}

	if (press) {
		m_MousePressedEvents[int(key)].m_Events.push_back(eventId);
	} else {
		m_MouseReleasedEvents[int(key)].m_Events.push_back(eventId);
	}
}

void df::InputSettings::RegisterScrollUpEvent(int eventId) {
	m_ScrollUpEvents.m_Events.push_back(eventId);
}

void df::InputSettings::RegisterScrollDownEvent(int eventId) {
	m_ScrollDownEvents.m_Events.push_back(eventId);
}

auto df::InputSettings::GetKeyPressedEvents() const -> const df::Vector<df::EventList>& {
	return m_KeyPressedEvents;
}

auto df::InputSettings::GetKeyReleasedEvents() const -> const df::Vector<df::EventList>& {
	return m_KeyReleasedEvents;
}

auto df::InputSettings::GetMousePressedEvents() const -> const df::Vector<df::EventList>& {
	return m_MousePressedEvents;
}

auto df::InputSettings::GetMouseReleasedEvents() const -> const df::Vector<df::EventList>& {
	return m_MouseReleasedEvents;
}

auto df::InputSettings::GetGamepadButtonPressedEvents() const -> const df::Vector<df::EventList>& {
	return m_GamepadButtonPressedEvents;
}

auto df::InputSettings::GetGamepadButtonReleasedEvents() const -> const df::Vector<df::EventList>& {
	return m_GamepadButtonReleasedEvents;
}

auto df::InputSettings::GetScrollUpEvents() const -> const df::EventList& {
	return m_ScrollUpEvents;
}

auto df::InputSettings::GetScrollDownEvents() const -> const df::EventList& {
	return m_ScrollDownEvents;
}

auto df::InputSettings::GetAxisMapping(int axis) const -> const AxisMapping& {
	DFAssert(axis < m_AxisMappings.size(), "Axis id is out of range!");

	return m_AxisMappings[axis];
}

auto df::InputSettings::GetAxisMapping(int axisId)->AxisMapping& {
	if (m_AxisMappings.size() <= axisId) {
		m_AxisMappings.resize(size_t(axisId) + 1);
	}

	return m_AxisMappings[axisId];
}