#pragma once

#include <DwarvenCore/Vector.h>

#include "VkAPI.h"

namespace rf {
	namespace api {
		struct ShaderModule {
			ShaderModule() = default;
			ShaderModule(const ShaderModule&) = default;
			ShaderModule(ShaderModule&&) = default;
			ShaderModule& operator = (const ShaderModule&) = default;
			ShaderModule& operator = (ShaderModule&&) = default;

			VkShaderModule m_Handle;
		};
	}
}