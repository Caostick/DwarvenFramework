#pragma once

#include "DwarfInput/InputDevice.h"
#include "DwarfInput/Keymaps.h"

#include <DwarvenCore/Bitset.h>

namespace df {
	class InputSettings;
}

namespace df {
	class StandardInputDevice : public InputDevice {
	public:
		StandardInputDevice(const InputSettings& inputSettings);

		void Update() override;

		auto GetCursorX() const -> double override;
		auto GetCursorY() const -> double override;

		auto GetAxis(int axis) const -> double override;

		void ProcessChar(char charCode);
		void ProcessKeyPress(InputKey key, bool repeat);
		void ProcessKeyRelease(InputKey key);
		void ProcessMousePress(InputMouseKey mouseKey);
		void ProcessMouseRelease(InputMouseKey mouseKey);
		void ProcessMouseMove(double x, double y);
		void ProcessScroll(int value);

	private:
		void SetKeyState(InputKey key, bool state);
		void SetMouseKeyState(InputMouseKey key, bool state);

		void SetKeyEaten(InputKey key, bool eaten);
		void SetMouseKeyEaten(InputMouseKey key, bool eaten);
		void SetScrollUpEaten(bool eaten);
		void SetScrollDownEaten(bool eaten);

		bool IsPressed(InputKey key) const;
		bool IsPressed(InputMouseKey mouseKey) const;

		auto GetMouseSpeedX() const -> double;
		auto GetMouseSpeedY() const -> double;

	private:
		const InputSettings& m_InputSettings;

		double m_MouseX;
		double m_MouseY;
		double m_MouseXPrev;
		double m_MouseYPrev;

		df::Bitset<int(InputKey::NUM_KEYS)> m_KeyStates;
		df::Bitset<int(InputKey::NUM_KEYS)> m_KeyEatenStates;
		df::Bitset<int(InputMouseKey::NUM_MOUSE_BUTTONS)> m_MouseKeyStates;
		df::Bitset<int(InputMouseKey::NUM_MOUSE_BUTTONS)> m_MouseKeyEatenStates;

		bool m_ScrollUpIsEaten;
		bool m_ScrollDownIsEaten;
	};
}