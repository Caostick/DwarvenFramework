#pragma once

#include "VkAPI.h"

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace vk {
	class ObjectManager;
	class CommandBuffer;
}

namespace vk {
	class TransferBuffer {
	public:
		TransferBuffer();

		void Init(VkDevice device, VkDeviceSize size);
		void Release(vk::ObjectManager& objectManager);

		void SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset = 0);
		void SetImageData(VkImage image, const void* data, uint32 dataSize, uint32 width, uint32 height, int32 widthOffset = 0, int32 heightOffset = 0, uint32 mip = 0);

		void Execute(vk::CommandBuffer& rcb);
	private:
		struct CopyToBufferTransaction {
			uint32 m_SrcBufferOffset;
			uint32 m_DstBufferOffset;
			uint32 m_DataSize;
			VkBuffer m_DstBuffer;
		};

		struct CopyToImageTransaction {
			uint32 m_SrcBufferOffset;
			uint32 m_DataSize;
			uint32 m_DstWidth;
			uint32 m_DstHeight;
			uint32 m_DstMip;
			int32 m_DstWidthOffset;
			int32 m_DstHeightOffset;
			VkImage m_DstImage;
			bool m_IsDepth;
			bool m_IsStencil;
		};

		VkDevice m_Device;
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;

		uint32 m_MemorySize;
		uint32 m_FreeMemory;

		df::Vector<CopyToBufferTransaction> m_CopyToBufferTransactions;
		df::Vector<CopyToImageTransaction> m_CopyToImageTransactions;
	};
}