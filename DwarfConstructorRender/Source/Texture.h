#pragma once

#include <DwarfConstructorRender/Types.h>

#include <DwarvenCore/String.h>

#include "Vulkan/VkTexture.h"

namespace rf {
	struct Texture {
		Texture() = default;
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator = (const Texture&) = default;
		Texture& operator = (Texture&&) = default;

		rf::api::Texture m_APIData;

		uint32 m_Width = 0;
		uint32 m_Height = 0;

		rf::ImageUsageFlags m_UsageFlags;
		rf::ETextureFormat m_Format;

		df::String m_Name;
	};
}