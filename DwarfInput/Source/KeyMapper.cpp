#include "DwarfInput/KeyMapper.h"

#include <DwarvenCore/New.h>

#include <GLFW/glfw3.h>

df::KeyMapper::KeyMapper() {
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_1] = InputMouseKey::MOUSE_BUTTON_1;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_2] = InputMouseKey::MOUSE_BUTTON_2;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_3] = InputMouseKey::MOUSE_BUTTON_3;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_4] = InputMouseKey::MOUSE_BUTTON_4;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_5] = InputMouseKey::MOUSE_BUTTON_5;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_6] = InputMouseKey::MOUSE_BUTTON_6;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_7] = InputMouseKey::MOUSE_BUTTON_7;
	m_MouseKeymaps[GLFW_MOUSE_BUTTON_8] = InputMouseKey::MOUSE_BUTTON_8;

	m_Keymaps[GLFW_KEY_SPACE] = InputKey::KEY_SPACE;
	m_Keymaps[GLFW_KEY_APOSTROPHE] = InputKey::KEY_APOSTROPHE;
	m_Keymaps[GLFW_KEY_COMMA] = InputKey::KEY_COMMA;
	m_Keymaps[GLFW_KEY_MINUS] = InputKey::KEY_MINUS;
	m_Keymaps[GLFW_KEY_PERIOD] = InputKey::KEY_PERIOD;
	m_Keymaps[GLFW_KEY_SLASH] = InputKey::KEY_SLASH;
	m_Keymaps[GLFW_KEY_0] = InputKey::KEY_0;
	m_Keymaps[GLFW_KEY_1] = InputKey::KEY_1;
	m_Keymaps[GLFW_KEY_2] = InputKey::KEY_2;
	m_Keymaps[GLFW_KEY_3] = InputKey::KEY_3;
	m_Keymaps[GLFW_KEY_4] = InputKey::KEY_4;
	m_Keymaps[GLFW_KEY_5] = InputKey::KEY_5;
	m_Keymaps[GLFW_KEY_6] = InputKey::KEY_6;
	m_Keymaps[GLFW_KEY_7] = InputKey::KEY_7;
	m_Keymaps[GLFW_KEY_8] = InputKey::KEY_8;
	m_Keymaps[GLFW_KEY_9] = InputKey::KEY_9;
	m_Keymaps[GLFW_KEY_SEMICOLON] = InputKey::KEY_SEMICOLON;
	m_Keymaps[GLFW_KEY_EQUAL] = InputKey::KEY_EQUAL;
	m_Keymaps[GLFW_KEY_A] = InputKey::KEY_A;
	m_Keymaps[GLFW_KEY_B] = InputKey::KEY_B;
	m_Keymaps[GLFW_KEY_C] = InputKey::KEY_C;
	m_Keymaps[GLFW_KEY_D] = InputKey::KEY_D;
	m_Keymaps[GLFW_KEY_E] = InputKey::KEY_E;
	m_Keymaps[GLFW_KEY_F] = InputKey::KEY_F;
	m_Keymaps[GLFW_KEY_G] = InputKey::KEY_G;
	m_Keymaps[GLFW_KEY_H] = InputKey::KEY_H;
	m_Keymaps[GLFW_KEY_I] = InputKey::KEY_I;
	m_Keymaps[GLFW_KEY_J] = InputKey::KEY_J;
	m_Keymaps[GLFW_KEY_K] = InputKey::KEY_K;
	m_Keymaps[GLFW_KEY_L] = InputKey::KEY_L;
	m_Keymaps[GLFW_KEY_M] = InputKey::KEY_M;
	m_Keymaps[GLFW_KEY_N] = InputKey::KEY_N;
	m_Keymaps[GLFW_KEY_O] = InputKey::KEY_O;
	m_Keymaps[GLFW_KEY_P] = InputKey::KEY_P;
	m_Keymaps[GLFW_KEY_Q] = InputKey::KEY_Q;
	m_Keymaps[GLFW_KEY_R] = InputKey::KEY_R;
	m_Keymaps[GLFW_KEY_S] = InputKey::KEY_S;
	m_Keymaps[GLFW_KEY_T] = InputKey::KEY_T;
	m_Keymaps[GLFW_KEY_U] = InputKey::KEY_U;
	m_Keymaps[GLFW_KEY_V] = InputKey::KEY_V;
	m_Keymaps[GLFW_KEY_W] = InputKey::KEY_W;
	m_Keymaps[GLFW_KEY_X] = InputKey::KEY_X;
	m_Keymaps[GLFW_KEY_Y] = InputKey::KEY_Y;
	m_Keymaps[GLFW_KEY_Z] = InputKey::KEY_Z;
	m_Keymaps[GLFW_KEY_LEFT_BRACKET] = InputKey::KEY_LEFT_BRACKET;
	m_Keymaps[GLFW_KEY_BACKSLASH] = InputKey::KEY_BACKSLASH;
	m_Keymaps[GLFW_KEY_RIGHT_BRACKET] = InputKey::KEY_RIGHT_BRACKET;
	m_Keymaps[GLFW_KEY_GRAVE_ACCENT] = InputKey::KEY_GRAVE_ACCENT;
	m_Keymaps[GLFW_KEY_WORLD_1] = InputKey::KEY_WORLD_1;
	m_Keymaps[GLFW_KEY_WORLD_2] = InputKey::KEY_WORLD_2;
	m_Keymaps[GLFW_KEY_ESCAPE] = InputKey::KEY_ESCAPE;
	m_Keymaps[GLFW_KEY_ENTER] = InputKey::KEY_ENTER;
	m_Keymaps[GLFW_KEY_TAB] = InputKey::KEY_TAB;
	m_Keymaps[GLFW_KEY_BACKSPACE] = InputKey::KEY_BACKSPACE;
	m_Keymaps[GLFW_KEY_INSERT] = InputKey::KEY_INSERT;
	m_Keymaps[GLFW_KEY_DELETE] = InputKey::KEY_DELETE;
	m_Keymaps[GLFW_KEY_RIGHT] = InputKey::KEY_RIGHT;
	m_Keymaps[GLFW_KEY_LEFT] = InputKey::KEY_LEFT;
	m_Keymaps[GLFW_KEY_DOWN] = InputKey::KEY_DOWN;
	m_Keymaps[GLFW_KEY_UP] = InputKey::KEY_UP;
	m_Keymaps[GLFW_KEY_PAGE_UP] = InputKey::KEY_PAGE_UP;
	m_Keymaps[GLFW_KEY_PAGE_DOWN] = InputKey::KEY_PAGE_DOWN;
	m_Keymaps[GLFW_KEY_HOME] = InputKey::KEY_HOME;
	m_Keymaps[GLFW_KEY_END] = InputKey::KEY_END;
	m_Keymaps[GLFW_KEY_CAPS_LOCK] = InputKey::KEY_CAPS_LOCK;
	m_Keymaps[GLFW_KEY_SCROLL_LOCK] = InputKey::KEY_SCROLL_LOCK;
	m_Keymaps[GLFW_KEY_NUM_LOCK] = InputKey::KEY_NUM_LOCK;
	m_Keymaps[GLFW_KEY_PRINT_SCREEN] = InputKey::KEY_PRINT_SCREEN;
	m_Keymaps[GLFW_KEY_PAUSE] = InputKey::KEY_PAUSE;
	m_Keymaps[GLFW_KEY_F1] = InputKey::KEY_F1;
	m_Keymaps[GLFW_KEY_F2] = InputKey::KEY_F2;
	m_Keymaps[GLFW_KEY_F3] = InputKey::KEY_F3;
	m_Keymaps[GLFW_KEY_F4] = InputKey::KEY_F4;
	m_Keymaps[GLFW_KEY_F5] = InputKey::KEY_F5;
	m_Keymaps[GLFW_KEY_F6] = InputKey::KEY_F6;
	m_Keymaps[GLFW_KEY_F7] = InputKey::KEY_F7;
	m_Keymaps[GLFW_KEY_F8] = InputKey::KEY_F8;
	m_Keymaps[GLFW_KEY_F9] = InputKey::KEY_F9;
	m_Keymaps[GLFW_KEY_F10] = InputKey::KEY_F10;
	m_Keymaps[GLFW_KEY_F11] = InputKey::KEY_F11;
	m_Keymaps[GLFW_KEY_F12] = InputKey::KEY_F12;
	m_Keymaps[GLFW_KEY_F13] = InputKey::KEY_F13;
	m_Keymaps[GLFW_KEY_F14] = InputKey::KEY_F14;
	m_Keymaps[GLFW_KEY_F15] = InputKey::KEY_F15;
	m_Keymaps[GLFW_KEY_F16] = InputKey::KEY_F16;
	m_Keymaps[GLFW_KEY_F17] = InputKey::KEY_F17;
	m_Keymaps[GLFW_KEY_F18] = InputKey::KEY_F18;
	m_Keymaps[GLFW_KEY_F19] = InputKey::KEY_F19;
	m_Keymaps[GLFW_KEY_F20] = InputKey::KEY_F20;
	m_Keymaps[GLFW_KEY_F21] = InputKey::KEY_F21;
	m_Keymaps[GLFW_KEY_F22] = InputKey::KEY_F22;
	m_Keymaps[GLFW_KEY_F23] = InputKey::KEY_F23;
	m_Keymaps[GLFW_KEY_F24] = InputKey::KEY_F24;
	m_Keymaps[GLFW_KEY_F25] = InputKey::KEY_F25;
	m_Keymaps[GLFW_KEY_KP_0] = InputKey::KEY_KP_0;
	m_Keymaps[GLFW_KEY_KP_1] = InputKey::KEY_KP_1;
	m_Keymaps[GLFW_KEY_KP_2] = InputKey::KEY_KP_2;
	m_Keymaps[GLFW_KEY_KP_3] = InputKey::KEY_KP_3;
	m_Keymaps[GLFW_KEY_KP_4] = InputKey::KEY_KP_4;
	m_Keymaps[GLFW_KEY_KP_5] = InputKey::KEY_KP_5;
	m_Keymaps[GLFW_KEY_KP_6] = InputKey::KEY_KP_6;
	m_Keymaps[GLFW_KEY_KP_7] = InputKey::KEY_KP_7;
	m_Keymaps[GLFW_KEY_KP_8] = InputKey::KEY_KP_8;
	m_Keymaps[GLFW_KEY_KP_9] = InputKey::KEY_KP_9;
	m_Keymaps[GLFW_KEY_KP_DECIMAL] = InputKey::KEY_KP_DECIMAL;
	m_Keymaps[GLFW_KEY_KP_DIVIDE] = InputKey::KEY_KP_DIVIDE;
	m_Keymaps[GLFW_KEY_KP_MULTIPLY] = InputKey::KEY_KP_MULTIPLY;
	m_Keymaps[GLFW_KEY_KP_SUBTRACT] = InputKey::KEY_KP_SUBTRACT;
	m_Keymaps[GLFW_KEY_KP_ADD] = InputKey::KEY_KP_ADD;
	m_Keymaps[GLFW_KEY_KP_ENTER] = InputKey::KEY_KP_ENTER;
	m_Keymaps[GLFW_KEY_KP_EQUAL] = InputKey::KEY_KP_EQUAL;
	m_Keymaps[GLFW_KEY_LEFT_SHIFT] = InputKey::KEY_LEFT_SHIFT;
	m_Keymaps[GLFW_KEY_LEFT_CONTROL] = InputKey::KEY_LEFT_CONTROL;
	m_Keymaps[GLFW_KEY_LEFT_ALT] = InputKey::KEY_LEFT_ALT;
	m_Keymaps[GLFW_KEY_LEFT_SUPER] = InputKey::KEY_LEFT_SUPER;
	m_Keymaps[GLFW_KEY_RIGHT_SHIFT] = InputKey::KEY_RIGHT_SHIFT;
	m_Keymaps[GLFW_KEY_RIGHT_CONTROL] = InputKey::KEY_RIGHT_CONTROL;
	m_Keymaps[GLFW_KEY_RIGHT_ALT] = InputKey::KEY_RIGHT_ALT;
	m_Keymaps[GLFW_KEY_RIGHT_SUPER] = InputKey::KEY_RIGHT_SUPER;
	m_Keymaps[GLFW_KEY_MENU] = InputKey::KEY_MENU;
}

auto df::KeyMapper::GetMappedKey(int code) -> InputKey {
	return m_Keymaps[code];
}

auto df::KeyMapper::GetMappedMouseKey(int code) -> InputMouseKey {
	return m_MouseKeymaps[code];
}
