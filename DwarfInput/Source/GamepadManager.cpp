#include "DwarfInput/GamepadManager.h"

#include <GLFW/glfw3.h>

Signal<int, bool> df::GamepadManager::OnGamepadConnectionChangedSignal;

namespace {
    static bool ToActivate[GLFW_JOYSTICK_LAST + 1];
    static bool ToDeactivate[GLFW_JOYSTICK_LAST + 1];
}

void df::GamepadManager::Init() {
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; ++i) {
        ToActivate[i] = (glfwJoystickPresent(i) == GLFW_TRUE);
        ToDeactivate[i] = false;
    }

    glfwSetJoystickCallback(&df::GamepadManager::OnGamepadConnectionChanged);
}

void df::GamepadManager::Release() {
    OnGamepadConnectionChangedSignal.DisconnectAll();
}

void df::GamepadManager::Update() {
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; ++i) {
        if (ToActivate[i]) {
            OnGamepadConnectionChangedSignal.Emit(i, true);
            ToActivate[i] = false;
        }

        if (ToDeactivate[i]) {
            OnGamepadConnectionChangedSignal.Emit(i, false);
            ToDeactivate[i] = false;
        }
    }
}

void df::GamepadManager::ReconnectGamepads() {
    for (int i = 0; i <= GLFW_JOYSTICK_LAST; ++i) {
        ToActivate[i] = (glfwJoystickPresent(i) == GLFW_TRUE);
    }
}

void df::GamepadManager::OnGamepadConnectionChanged(int index, int connection) {
	if (connection == GLFW_CONNECTED) {
		printf("Gamepad[%s] connected!\n", glfwGetJoystickName(index));
		ToActivate[index] = true;
	} else if (connection == GLFW_DISCONNECTED) {
		ToDeactivate[index] = true;
	}
}