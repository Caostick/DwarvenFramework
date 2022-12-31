#include "DwarfInput/Input.h"
#include "DwarfInput/GamepadManager.h"

#include <GLFW/glfw3.h>

#include <DwarfPlatform/Window.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

df::Input::Input() 
	: m_StandardInputDevice(m_InputSettings) 
	, m_ActiveInputDevice(nullptr) {
}

void df::Input::Init(Window* window) {
	if (window) {
		window->OnKeySignal.Connect(this, &Input::KeyFunction);
		window->OnCharSignal.Connect(this, &Input::CharFunction);
		window->OnMouseButtonSignal.Connect(this, &Input::MouseButtonFunction);
		window->OnScrollSignal.Connect(this, &Input::ScrollFunction);
		window->OnMouseMoveSignal.Connect(this, &Input::MouseMoveFunction);
	}

	m_InputDevices.push_back(&m_StandardInputDevice);
	m_ActiveInputDevice = &m_StandardInputDevice;

	GamepadManager::Init();
	GamepadManager::OnGamepadConnectionChangedSignal.Connect(this, &df::Input::GamepadConnectionFunction);
}

void df::Input::Release() {
	m_ActiveInputDevice = nullptr;
	m_InputDevices.clear();

	for (auto inputDevice : m_GamepadInputDevices) {
		DFDelete inputDevice;
	}
	m_GamepadInputDevices.clear();

	GamepadManager::Release();
}

void df::Input::Update() {
	GamepadManager::Update();

	bool activeDeviceIsValid = false;

	for (auto inputDevice : m_InputDevices) {
		inputDevice->Update();

		if (!inputDevice->GetEvents().empty()) {
			m_ActiveInputDevice = inputDevice;
		}

		if(m_ActiveInputDevice == inputDevice) {
			activeDeviceIsValid = true;
		}
	}

	if(!activeDeviceIsValid) {
		m_ActiveInputDevice = &m_StandardInputDevice;
	}
}

void df::Input::Reset() {
	m_StandardInputDevice.ResetEvents();
	for (auto inputDevice : m_GamepadInputDevices) {
		if (inputDevice) {
			inputDevice->ResetEvents();
		}
	}
}

auto df::Input::GetActiveInputDevice() const -> const df::InputDevice* {
	return m_ActiveInputDevice;
}

auto df::Input::GetInputSettings()->df::InputSettings& {
	return m_InputSettings;
}

void df::Input::KeyFunction(int key, int /*scancode*/, int state, int /*mods*/) {
	switch (state) {
	case 0:
		m_StandardInputDevice.ProcessKeyRelease(m_KeyMapper.GetMappedKey(key));
		break;
	case 1:
		m_StandardInputDevice.ProcessKeyPress(m_KeyMapper.GetMappedKey(key), false);
		break;
	case 2:
		m_StandardInputDevice.ProcessKeyPress(m_KeyMapper.GetMappedKey(key), true);
		break;
	default:
		break;
	}
}

void df::Input::CharFunction(unsigned int charCode) {
	m_StandardInputDevice.ProcessChar(static_cast<char>(charCode));
}

void df::Input::MouseButtonFunction(int button, int state, int /*mods*/) {
	switch (state) {
	case 0:
		m_StandardInputDevice.ProcessMouseRelease(m_KeyMapper.GetMappedMouseKey(button));
		break;
	case 1:
		m_StandardInputDevice.ProcessMousePress(m_KeyMapper.GetMappedMouseKey(button));
		break;
	default:
		break;
	}
}

void df::Input::ScrollFunction(double /*xOffset*/, double yOffset) {
	m_StandardInputDevice.ProcessScroll(static_cast<int>(yOffset));
}

void df::Input::MouseMoveFunction(double x, double y) {
	m_StandardInputDevice.ProcessMouseMove(x, y);
}

void df::Input::GamepadConnectionFunction(int gamepadIdx, bool connected) {
	if (m_GamepadInputDevices.size() >= gamepadIdx) {
		m_GamepadInputDevices.resize(gamepadIdx + 1);
	}

	if (connected) {
		m_GamepadInputDevices[gamepadIdx] = DFNew GamepadInputDevice(m_InputSettings, gamepadIdx);
		m_InputDevices.push_back(m_GamepadInputDevices[gamepadIdx]);

	} else {
		df::RemoveUnordered(m_InputDevices, (df::InputDevice*)m_GamepadInputDevices[gamepadIdx]);

		DFDelete m_GamepadInputDevices[gamepadIdx];
		m_GamepadInputDevices[gamepadIdx] = nullptr;
	}
}