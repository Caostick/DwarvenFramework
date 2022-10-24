#pragma once

#include "VkAPI.h"

#include <DwarfRender/Texture.h>

#include <DwarvenCore/String.h>

namespace vk {
	class RenderCore;
}

namespace vk {
	auto ToVkImageUsageFlags(df::ImageUsageFlags flags)->VkImageUsageFlags;
	auto ToVkFormat(df::ETextureFormat format)->VkFormat;
}

namespace vk {
	class Texture : public df::Texture {
	public:
		Texture(vk::RenderCore& renderCore);

		virtual ~Texture() override;

		virtual void SetName(const df::StringView& name) override;
		virtual void Create(uint32 width, uint32 height, df::ETextureFormat format, bool mips, df::ImageUsageFlags usage) override;
		virtual void SetData(void* data, uint32 size) override;
		virtual void GenerateMips() override;
		virtual auto GetWidth() const->uint32 override;
		virtual auto GetHeight() const->uint32 override;
		virtual auto GetMips() const->uint32 override;

	public:
		bool IsDepthStencil() const;
		bool IsDepth() const;
		bool IsStencil() const;

		auto GetVkImage() const->VkImage;
		auto GetVkImageView() const->VkImageView;
		auto GetVkDeviceMemory() const->VkDeviceMemory;

	private:

	private:
		vk::RenderCore& m_RenderCore;

		df::String m_Name;

		uint32 m_Width;
		uint32 m_Height;

		uint32 m_Mips;

		df::ETextureFormat m_Format;
		df::ImageUsageFlags m_UsageFlags;

		VkImage m_VkImage;
		VkImageView m_VkImageView;
		VkDeviceMemory m_VkMemory;
		VkImageUsageFlags m_VkUsageFlags;
		VkFormat m_VkFormat;
	};
}