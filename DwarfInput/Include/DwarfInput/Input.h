#pragma once

#include "DwarfInput/KeyMapper.h"
#include "DwarfInput/KeyMaps.h"
#include "DwarfInput/InputSettings.h"
#include "DwarfInput/StandardInputDevice.h"
#include "DwarfInput/GamepadInputDevice.h"

#include <DwarvenCore/Vector.h>

namespace df {
	class Window;
}

namespace df {
	class Input {
	public:
		Input();

		void Init(Window* window);
		void Release();
		void Update();
		void Reset();

		void KeyFunction(int key, int scancode, int state, int mods);
		void CharFunction(unsigned int charCode);
		void MouseButtonFunction(int button, int state, int mods);
		void ScrollFunction(double xOffset, double yOffset);
		void MouseMoveFunction(double x, double y);
		void GamepadConnectionFunction(int gamepadIdx, bool connected);

		auto GetActiveInputDevice() const -> const df::InputDevice*;
		auto GetInputSettings()->df::InputSettings&;

	private:
		df::StandardInputDevice m_StandardInputDevice;
		df::Vector<df::GamepadInputDevice*> m_GamepadInputDevices;

		df::Vector<df::InputDevice*> m_InputDevices;
		df::InputDevice* m_ActiveInputDevice;

		df::KeyMapper m_KeyMapper;
		df::InputSettings m_InputSettings;
	};
}