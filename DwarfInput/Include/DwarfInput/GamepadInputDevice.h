#pragma once

#include "DwarfInput/InputDevice.h"
#include "DwarfInput/GamepadRedirectionSettings.h"

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Bitset.h>

namespace df {
	class InputSettings;
}

namespace df {
	class GamepadInputDevice : public InputDevice {
	public:
		GamepadInputDevice(const InputSettings& inputSettings, int gamepadId);

		auto GetCursorX() const -> double override;
		auto GetCursorY() const -> double override;
		auto GetAxis(int axis) const -> double override;

		void Update() override;
		auto GetAxis(EGamepadAxis axis) const -> float;
		auto GetIndex() const->int;

	private:
		bool IsPressed(EGamepadButton gamepadButton) const;
		void ProcessButton(EGamepadButton button, bool press);
		auto RemapAxisValue(float value, float boundA, float boundB) const -> float;
		auto CalibrateAxisValue(float value) const -> float;

	private:
		const InputSettings& m_InputSettings;
		int m_GamepadId;

		GamepadRedirectionSettings m_RedirectionSettings;

		uint8 m_ButtonStates[GamepadButtonCount];
		float m_AxisStates[GamepadButtonCount];

		df::Bitset<int(EGamepadButton::NumButtons)> m_GamepadButtonStates;
		df::Bitset<int(EGamepadButton::NumButtons)> m_GamepadButtonEatenStates;

		double m_CursorX;
		double m_CursorY;
	};
}