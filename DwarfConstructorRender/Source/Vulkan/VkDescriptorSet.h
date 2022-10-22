#pragma once

#include "VkAPI.h"

#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Vector.h>

namespace rf {
	namespace api {
		struct DescriptorSet {
			VkDevice m_Device;
			VkDescriptorSet m_Handle;
		};
	}
}