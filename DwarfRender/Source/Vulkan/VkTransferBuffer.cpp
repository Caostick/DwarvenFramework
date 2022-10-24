#include "VkTransferBuffer.h"
#include "VkObjectManager.h"
#include "VkHelper.h"
#include "VkDebug.h"
#include "VkCommandBuffer.h"

#include <DwarvenCore/Assert.h>

vk::TransferBuffer::TransferBuffer() 
	: m_Device(VK_NULL_HANDLE)
	, m_Buffer(VK_NULL_HANDLE)
	, m_Memory(VK_NULL_HANDLE)
	, m_MemorySize(0)
	, m_FreeMemory(0) {

}

void vk::TransferBuffer::Init(VkDevice device, VkDeviceSize size) {
	m_Device = device;
	m_MemorySize = uint32(size);
	m_FreeMemory = m_MemorySize;

	m_Buffer = vk::CreateBuffer(m_Device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
	DFVkDebugName(m_Device, m_Buffer, "Transfer Buffer");

	m_Memory = vk::AllocateMemory(m_Device, m_Buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	DFVkDebugName(m_Device, m_Memory, "Transfer Buffer Memory");

	vk::BindBufferMemory(m_Device, m_Buffer, m_Memory);
}

void vk::TransferBuffer::Release(vk::ObjectManager& objectManager) {
	objectManager.RemoveBuffer(m_Device, m_Buffer);
	objectManager.RemoveDeviceMemory(m_Device, m_Memory);
}

void vk::TransferBuffer::SetBufferData(VkBuffer buffer, const void* data, uint32 dataSize, uint32 offset /*= 0*/) {
	DFAssert(dataSize < m_FreeMemory, "Not enough memory in transfer buffer!");

	CopyToBufferTransaction transaction;
	transaction.m_SrcBufferOffset = m_MemorySize - m_FreeMemory;
	transaction.m_DataSize = dataSize;
	transaction.m_DstBufferOffset = offset;
	transaction.m_DstBuffer = buffer;
	m_CopyToBufferTransactions.emplace_back(transaction);

	vk::SetData(m_Device, m_Memory, data, dataSize, m_MemorySize - m_FreeMemory);
	m_FreeMemory -= dataSize;
}

void vk::TransferBuffer::SetImageData(VkImage image, const void* data, uint32 dataSize, uint32 width, uint32 height, int32 widthOffset /*= 0*/, int32 heightOffset /*= 0*/) {
	DFAssert(dataSize < m_FreeMemory, "Not enough memory in transfer buffer!");

	CopyToImageTransaction transaction;
	transaction.m_SrcBufferOffset = m_MemorySize - m_FreeMemory;
	transaction.m_DataSize = dataSize;
	transaction.m_DstWidth = width;
	transaction.m_DstHeight = height;
	transaction.m_DstWidthOffset = widthOffset;
	transaction.m_DstHeightOffset = heightOffset;
	transaction.m_DstImage = image;

	m_CopyToImageTransactions.emplace_back(transaction);

	vk::SetData(m_Device, m_Memory, data, dataSize, m_MemorySize - m_FreeMemory);
	m_FreeMemory -= dataSize;
}

void vk::TransferBuffer::Execute(vk::CommandBuffer& rcb) {
	DFScopedRenderEvent(rcb, "Execute Data Transfer");

	for (const auto& transaction : m_CopyToBufferTransactions) {
		rcb.CopyBuffer(m_Buffer, transaction.m_DstBuffer, transaction.m_DataSize, transaction.m_SrcBufferOffset, transaction.m_DstBufferOffset);
	}

	for (const auto& transaction : m_CopyToImageTransactions) {
		rcb.ImageLayoutTransition(transaction.m_DstImage, vk::EImageLayout::ColorReadOnly, vk::EImageLayout::TransferDst);
		rcb.CopyBufferToImage(m_Buffer, transaction.m_DstImage, transaction.m_DstWidth, transaction.m_DstHeight, transaction.m_DstWidthOffset, transaction.m_DstHeightOffset);
		rcb.ImageLayoutTransition(transaction.m_DstImage, vk::EImageLayout::TransferDst, vk::EImageLayout::ColorReadOnly);
	}

	m_CopyToBufferTransactions.clear();
	m_CopyToImageTransactions.clear();
	m_FreeMemory = m_MemorySize;
}