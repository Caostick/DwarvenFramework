#include "VkTexture.h"
#include "VkRenderCore.h"
#include "VkHelper.h"
#include "VkDebug.h"

auto vk::ToVkImageUsageFlags(df::ImageUsageFlags flags)->VkImageUsageFlags {
	VkImageUsageFlags imageUsageFlags = 0;

	if (flags.Has(df::EImageUsageFlag::Texture)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	if (flags.Has(df::EImageUsageFlag::RenderTarget)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}

	if (flags.Has(df::EImageUsageFlag::DepthRenderTarget)) {
		imageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}

	if (flags.Has(df::EImageUsageFlag::Mips)) {
		imageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	}

	return imageUsageFlags;
}

auto vk::ToVkFormat(df::ETextureFormat format)->VkFormat {
	switch (format) {
	case df::ETextureFormat::D32_Float:
		return VK_FORMAT_D32_SFLOAT;
	case df::ETextureFormat::D24_Float_S8_UInt:
		return VK_FORMAT_D24_UNORM_S8_UINT;

	case df::ETextureFormat::R8_UNorm:
		return VK_FORMAT_R8_UNORM;
	case df::ETextureFormat::R16_UNorm:
		return VK_FORMAT_R16_UNORM;
	case df::ETextureFormat::R8G8_UNorm:
		return VK_FORMAT_R8G8_UNORM;
	case df::ETextureFormat::R8G8B8A8_UNorm:
		return VK_FORMAT_R8G8B8A8_UNORM;
	case df::ETextureFormat::A2R10G10B10_UNorm:
		return VK_FORMAT_A2R10G10B10_UNORM_PACK32;

	case df::ETextureFormat::R16_Float:
		return VK_FORMAT_R16_SFLOAT;
	case df::ETextureFormat::R32_Float:
		return VK_FORMAT_R32_SFLOAT;
	case df::ETextureFormat::R16G16_Float:
		return VK_FORMAT_R16G16_SFLOAT;
	case df::ETextureFormat::R16G16B16A16_Float:
		return VK_FORMAT_R16G16B16A16_SFLOAT;

	default:
		return VK_FORMAT_UNDEFINED;
	}
}

vk::Texture::Texture(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore)
	, m_Width(0)
	, m_Height(0)
	, m_Format(df::ETextureFormat::Undefined)
	, m_UsageFlags()
	, m_VkImage(VK_NULL_HANDLE)
	, m_VkImageView(VK_NULL_HANDLE)
	, m_VkMemory(VK_NULL_HANDLE)
	, m_VkFormat(VK_FORMAT_UNDEFINED) {
}

vk::Texture::~Texture() {
	if (m_VkImageView != VK_NULL_HANDLE) {
		m_RenderCore.RemoveImageView(m_VkImageView);
	}

	if (m_VkImage != VK_NULL_HANDLE) {
		m_RenderCore.RemoveImage(m_VkImage);
	}

	if (m_VkMemory != VK_NULL_HANDLE) {
		m_RenderCore.RemoveDeviceMemory(m_VkMemory);
	}
}

void vk::Texture::SetName(const df::StringView& name) {
	m_Name = df::String(name);

	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	if (m_VkImage != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkImage, m_Name);
	}

	if (m_VkImageView != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkImageView, m_Name);
	}

	if (m_VkMemory != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkMemory, m_Name);
	}
}

void vk::Texture::Create(uint32 width, uint32 height, df::ETextureFormat format, df::ImageUsageFlags usage) {
	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	m_Width = width;
	m_Height = height;
	m_Format = format;
	m_UsageFlags = usage;
	m_Mips = 1 + (usage.Has(df::EImageUsageFlag::Mips) ? static_cast<uint32>(std::log2(std::max(width, height))) : 0);

	m_VkUsageFlags = ToVkImageUsageFlags(m_UsageFlags);
	m_VkFormat = ToVkFormat(m_Format);

	m_VkImage = vk::CreateImage2D(vkDevice, m_Width, m_Height, m_VkFormat, m_VkUsageFlags, m_Mips);
	DFVkDebugName(vkDevice, m_VkImage, m_Name);

	m_VkMemory = vk::AllocateMemory(vkDevice, m_VkImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	DFVkDebugName(vkDevice, m_VkMemory, m_Name);

	vk::BindImageMemory(vkDevice, m_VkImage, m_VkMemory);
	m_VkImageView = vk::CreateImageView(vkDevice, m_VkImage, m_VkFormat, m_Mips);
	DFVkDebugName(vkDevice, m_VkImageView, m_Name);
}

void vk::Texture::SetData(void* data, uint32 size) {
	DFAssert(m_VkImage != VK_NULL_HANDLE, "Image is not created!");
	m_RenderCore.SetImageData(m_VkImage, data, size, m_Width, m_Height);
}

void vk::Texture::GenerateMips() {
	// @TODO: Check if image format supports linear blitting
	/*VkFormatProperties formatProperties;
	vk::API::GetPhysicalDeviceFormatProperties(physicalDevice, m_VkFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		DFAssert(false, "Texture image format does not support linear blitting!");
	}*/
	m_RenderCore.GenerateMips(this);
}

auto vk::Texture::GetWidth() const->uint32 {
	return m_Width;
}

auto vk::Texture::GetHeight() const->uint32 {
	return m_Height;
}

auto vk::Texture::GetMips() const->uint32 {
	return m_Mips;
}

bool vk::Texture::IsDepthStencil() const {
	return
		m_VkFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
		m_VkFormat == VK_FORMAT_D32_SFLOAT ||
		m_VkFormat == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

bool vk::Texture::IsDepth() const {
	return
		m_VkFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
		m_VkFormat == VK_FORMAT_D32_SFLOAT ||
		m_VkFormat == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

bool vk::Texture::IsStencil() const {
	return
		m_VkFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
		m_VkFormat == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

auto vk::Texture::GetVkImage() const->VkImage {
	return m_VkImage;
}

auto vk::Texture::GetVkImageView() const->VkImageView {
	return m_VkImageView;
}

auto vk::Texture::GetVkDeviceMemory() const->VkDeviceMemory {
	return m_VkMemory;
}

auto vk::Texture::GetVkFormat() const->VkFormat {
	return m_VkFormat;
}