#pragma once

#include "DwarfInput/Keymaps.h"

namespace df {
	class KeyMapper {
	public:

		KeyMapper();

		auto GetMappedKey(int code) -> InputKey;
		auto GetMappedMouseKey(int code) -> InputMouseKey;

	private:
		InputKey            m_Keymaps[512];
		InputMouseKey       m_MouseKeymaps[16];
	};
}