#pragma once

#include "VkAPI.h"

#include <DwarfRender/Buffer.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/EnumFlags.h>

namespace vk {
	class RenderCore;
}

namespace vk {
	enum class EBufferAccessType {
		Transfer,
		Direct
	};

	enum class EBufferUsageFlag {
		Storage = 1 << 0,
		Uniform = 1 << 1,
		StorageTexel = 1 << 2,
		UniformTexel = 1 << 3,
		Vertex = 1 << 4,
		Index = 1 << 5,
		Indirect = 1 << 6
	};

	using BufferUsageFlags = df::EnumFlags<EBufferUsageFlag>;
	DF_DEFINE_ENUM_FLAG_OPERATORS(BufferUsageFlags);
}

namespace vk {
	class Buffer : public df::Buffer {
	public:
		Buffer(vk::RenderCore& renderCore);
		
		virtual ~Buffer() override;

		virtual void SetName(const df::StringView& name) override;

		virtual void Create(uint32 size) override;
		virtual void SetData(const void* data, uint32 size, uint32 offset = 0) override;

	public:
		auto GetVkBuffer() const->VkBuffer;
		auto GetVkBufferUsageFlags() const->VkBufferUsageFlags;
		auto GetVkDeviceSize() const->VkDeviceSize;
		auto GetVkDeviceMemory() const->VkDeviceMemory;
		auto GetVkMemoryPropertyFlags() const->VkMemoryPropertyFlags;

		void CreateBuffer(uint32 size, const BufferUsageFlags& usageFlags);

	private:
		void UpdateDebugNames();

	private:
		vk::RenderCore& m_RenderCore;

		df::String m_Name;

		uint32 m_Size;
		uint32 m_Stride;
		uint32 m_UpdateSize;
		BufferUsageFlags m_UsageFlags;
		EBufferAccessType m_AccessType;

		VkBuffer m_VkBuffer;
		VkBufferUsageFlags m_VkBufferUsageFlags;
		VkDeviceSize m_VkBufferSize;
		VkDeviceMemory m_VkMemory;
		VkMemoryPropertyFlags m_VkMemoryPropertyFlags;
	};
}