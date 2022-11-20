#pragma once

#include "VkAPI.h"

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace vk {
	class ObjectManager;
}

namespace rf {
	class CommandBuffer;
}

namespace rf {
	namespace api {
		class TransferBuffer {
		public:
			TransferBuffer();

			void Init(VkDevice device, VkDeviceSize size);
			void Release(vk::ObjectManager& objectManager);

			void SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset = 0);

			void Execute(rf::CommandBuffer& rcb);
		private:
			struct Transaction {
				uint32 m_SrcBufferOffset;
				uint32 m_DstBufferOffset;
				uint32 m_DataSize;
				VkBuffer m_DstBuffer;
			};

			VkDevice m_Device;
			VkBuffer m_Buffer;
			VkDeviceMemory m_Memory;

			uint32 m_MemorySize;
			uint32 m_FreeMemory;

			df::Vector<Transaction> m_Transactions;
		};
	}
}