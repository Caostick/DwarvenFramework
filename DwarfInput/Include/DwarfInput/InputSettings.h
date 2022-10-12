#pragma once

#include "DwarfInput/Keymaps.h"

#include <DwarvenCore/Vector.h>

namespace df {
	enum class EInpitAxisType {
		MouseSpeedX,
		MouseSpeedY,
		MouseButton,
		GamepadButton,
		GamepadAxis,
		Key,
	};

	struct AxisInputMapping {
		AxisInputMapping() = default;
		AxisInputMapping(const AxisInputMapping&) = default;
		AxisInputMapping(AxisInputMapping&&) = default;
		AxisInputMapping& operator = (const AxisInputMapping&) = default;
		AxisInputMapping& operator = (AxisInputMapping&&) = default;

		EInpitAxisType m_Type;
		InputKey m_Key = InputKey::KEY_UNKNOWN;
		InputMouseKey m_MouseKey = InputMouseKey::MOUSE_BUTTON_UNKNOWN;
		EGamepadButton m_GamepadButton = EGamepadButton::Unknown;
		EGamepadAxis m_GamepadAxis = EGamepadAxis::Unknown;

		double m_Scale = 1.0f;
	};

	struct AxisMapping {
		AxisMapping() = default;
		AxisMapping(const AxisMapping&) = default;
		AxisMapping(AxisMapping&&) = default;
		AxisMapping& operator = (const AxisMapping&) = default;
		AxisMapping& operator = (AxisMapping&&) = default;

		df::Vector<AxisInputMapping> m_InputMappings;
	};

	struct EventList {
		EventList() = default;
		EventList(const EventList&) = default;
		EventList(EventList&&) = default;
		EventList& operator = (const EventList&) = default;
		EventList& operator = (EventList&&) = default;

		df::Vector<int> m_Events;
	};

	class InputSettings {
	public:

		void Reset();

		void RegisterAxis(int axisId, double scale, InputKey key);
		void RegisterAxis(int axisId, double scale, InputMouseKey key);
		void RegisterAxis(int axisId, double scale, EGamepadButton button);
		void RegisterAxis(int axisId, double scale, EGamepadAxis axis);
		void RegisterMouseSpeedXAxis(int axisId, double scale);
		void RegisterMouseSpeedYAxis(int axisId, double scale);
		void RegisterGamepadButtonEvent(EGamepadButton button, bool press, int eventId);
		void RegisterInputEvent(InputKey key, bool press, int eventId);
		void RegisterInputMouseEvent(InputMouseKey key, bool press, int eventId);
		void RegisterScrollUpEvent(int eventId);
		void RegisterScrollDownEvent(int eventId);

		auto GetKeyPressedEvents() const -> const df::Vector<df::EventList>&;
		auto GetKeyReleasedEvents() const -> const df::Vector<df::EventList>&;
		auto GetMousePressedEvents() const -> const df::Vector<df::EventList>&;
		auto GetMouseReleasedEvents() const -> const df::Vector<df::EventList>&;
		auto GetGamepadButtonPressedEvents() const -> const df::Vector<df::EventList>&;
		auto GetGamepadButtonReleasedEvents() const -> const df::Vector<df::EventList>&;
		auto GetScrollUpEvents() const -> const df::EventList&;
		auto GetScrollDownEvents() const -> const df::EventList&;

		auto GetAxisMapping(int axis) const -> const AxisMapping&;
		auto GetAxisMapping(int axisId)->AxisMapping&;

	private:
		df::Vector<df::EventList> m_KeyPressedEvents;
		df::Vector<df::EventList> m_KeyReleasedEvents;
		df::Vector<df::EventList> m_MousePressedEvents;
		df::Vector<df::EventList> m_MouseReleasedEvents;
		df::Vector<df::EventList> m_GamepadButtonPressedEvents;
		df::Vector<df::EventList> m_GamepadButtonReleasedEvents;
		df::EventList m_ScrollUpEvents;
		df::EventList m_ScrollDownEvents;

		df::Vector<AxisMapping> m_AxisMappings;
	};
}