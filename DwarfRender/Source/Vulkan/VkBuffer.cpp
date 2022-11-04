#include "VkBuffer.h"
#include "VkRenderCore.h"
#include "VkHelper.h"
#include "VkDebug.h"

#include <DwarvenCore/Memory.h>

namespace {
	auto ToVkBufferUsageFlags(vk::BufferUsageFlags flags)->VkBufferUsageFlags {
		VkBufferUsageFlags bufferUsageFlags = 0;

		if (flags.Has(vk::EBufferUsageFlag::Index)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::Storage)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::Uniform)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::StorageTexel)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::UniformTexel)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::Vertex)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}

		if (flags.Has(vk::EBufferUsageFlag::Indirect)) {
			bufferUsageFlags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		}

		return bufferUsageFlags;
	}
}

vk::Buffer::Buffer(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore)
	, m_VkBuffer(VK_NULL_HANDLE)
	, m_VkBufferUsageFlags(0)
	, m_VkBufferSize(0)
	, m_VkMemory(VK_NULL_HANDLE)
	, m_VkMemoryPropertyFlags(0) 
	, m_AccessType(EBufferAccessType::Transfer)
	, m_Size(0)
	, m_Stride(0)
	, m_UpdateSize(0) {
}

vk::Buffer::~Buffer() {
	if (m_VkBuffer != VK_NULL_HANDLE) {
		m_RenderCore.RemoveBuffer(m_VkBuffer);
	}

	if (m_VkMemory != VK_NULL_HANDLE) {
		m_RenderCore.RemoveDeviceMemory(m_VkMemory);
	}
}

void vk::Buffer::SetName(const df::StringView& name) {
	m_Name = df::String(name);

	UpdateDebugNames();
}

void vk::Buffer::Create(uint32 size) {
	CreateBuffer(size, vk::EBufferUsageFlag::Storage);
}

void vk::Buffer::SetData(const void* data, uint32 size, uint32 offset /*= 0*/) {
	if (m_AccessType == vk::EBufferAccessType::Transfer) {
		m_RenderCore.SetBufferData(m_VkBuffer, data, size, offset);
	} else {
		const VkDevice vkDevice = m_RenderCore.GetVkDevice();

		vk::SetData(vkDevice, m_VkMemory, data, size, offset);
	}
}

auto vk::Buffer::GetVkBuffer() const->VkBuffer {
	return m_VkBuffer;
}

auto vk::Buffer::GetVkBufferUsageFlags() const->VkBufferUsageFlags {
	return m_VkBufferUsageFlags;
}

auto vk::Buffer::GetVkDeviceSize() const->VkDeviceSize {
	return m_VkBufferSize;
}

auto vk::Buffer::GetVkDeviceMemory() const->VkDeviceMemory {
	return m_VkMemory;
}

auto vk::Buffer::GetVkMemoryPropertyFlags() const->VkMemoryPropertyFlags {
	return m_VkMemoryPropertyFlags;
}

void vk::Buffer::CreateBuffer(uint32 size, const BufferUsageFlags& usageFlags) {
	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	m_UsageFlags = usageFlags;
	m_AccessType = m_UsageFlags.Has(EBufferUsageFlag::Uniform) ? vk::EBufferAccessType::Direct : vk::EBufferAccessType::Transfer;
	m_Size = size;

	VkBufferUsageFlags bufferUsageFlags = ToVkBufferUsageFlags(m_UsageFlags);
	VkMemoryPropertyFlags memoryPropertyFlags = 0;

	switch (m_AccessType) {
	case vk::EBufferAccessType::Transfer:
		bufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		memoryPropertyFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		break;
	case vk::EBufferAccessType::Direct:
		memoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		break;
	default:
		break;
	}

	const uint32 dataStride = df::MemAlign(m_Size, m_RenderCore.GetBufferAlignment(usageFlags));

	m_Stride = dataStride;
	m_UpdateSize = size;

	m_VkBufferSize = dataStride;
	m_VkBufferUsageFlags = bufferUsageFlags;
	m_VkMemoryPropertyFlags = memoryPropertyFlags;

	m_VkBuffer = vk::CreateBuffer(vkDevice, m_VkBufferSize, m_VkBufferUsageFlags, VK_SHARING_MODE_EXCLUSIVE);
	m_VkMemory = vk::AllocateMemory(vkDevice, m_VkBuffer, m_VkMemoryPropertyFlags);

	vk::BindBufferMemory(vkDevice, m_VkBuffer, m_VkMemory);

	UpdateDebugNames();
}

void vk::Buffer::UpdateDebugNames() {
	const VkDevice vkDevice = m_RenderCore.GetVkDevice();

	if (m_VkBuffer != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkBuffer, m_Name);
	}

	if (m_VkMemory != VK_NULL_HANDLE) {
		DFVkDebugName(vkDevice, m_VkMemory, m_Name);
	}
}