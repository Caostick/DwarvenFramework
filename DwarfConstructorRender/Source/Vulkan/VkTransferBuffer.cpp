#include "VkTransferBuffer.h"
#include "VkObjectManager.h"
#include "VkHelper.h"
#include "VkDebug.h"

#include "../CommandBuffer.h"

#include <DwarvenCore/Assert.h>

rf::api::TransferBuffer::TransferBuffer() 
	: m_Device(VK_NULL_HANDLE)
	, m_Buffer(VK_NULL_HANDLE)
	, m_Memory(VK_NULL_HANDLE)
	, m_MemorySize(0)
	, m_FreeMemory(0) {

}

void rf::api::TransferBuffer::Init(VkDevice device, VkDeviceSize size) {
	m_Device = device;
	m_MemorySize = uint32(size);
	m_FreeMemory = m_MemorySize;

	m_Buffer = vk::CreateBuffer(m_Device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
	DFVkDebugName(m_Device, m_Buffer, "Transfer Buffer");

	m_Memory = vk::AllocateMemory(m_Device, m_Buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	DFVkDebugName(m_Device, m_Memory, "Transfer Buffer Memory");

	vk::BindBufferMemory(m_Device, m_Buffer, m_Memory);
}

void rf::api::TransferBuffer::Release(vk::ObjectManager& objectManager) {
	objectManager.RemoveBuffer(m_Device, m_Buffer);
	objectManager.RemoveDeviceMemory(m_Device, m_Memory);
}

void rf::api::TransferBuffer::SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset /*= 0*/) {
	DFAssert(dataSize < m_FreeMemory, "Not enough memory in transfer buffer!");

	Transaction transaction;
	transaction.m_SrcBufferOffset = m_MemorySize - m_FreeMemory;
	transaction.m_DataSize = dataSize;
	transaction.m_DstBufferOffset = offset;
	transaction.m_DstBuffer = buffer;
	m_Transactions.emplace_back(transaction);

	vk::SetData(m_Device, m_Memory, data, dataSize, m_MemorySize - m_FreeMemory);
	m_FreeMemory -= dataSize;
}

void rf::api::TransferBuffer::Execute(rf::CommandBuffer& rcb) {
	DFScopedRenderEvent(rcb, "Execute Data Transfer");

	for (const auto& transaction : m_Transactions) {
		rcb.GetAPIData().CopyBuffer(m_Buffer, transaction.m_DstBuffer, transaction.m_DataSize, transaction.m_SrcBufferOffset, transaction.m_DstBufferOffset);
	}

	m_Transactions.clear();
	m_FreeMemory = m_MemorySize;
}