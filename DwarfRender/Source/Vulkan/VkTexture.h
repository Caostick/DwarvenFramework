#pragma once

#include "VkAPI.h"

#include <DwarfRender/Texture.h>

namespace vk {
	class Texture : public df::Texture {
	public:


	private:
		VkImage m_VkImage;
	};
}