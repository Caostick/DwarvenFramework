#pragma once

#include "VkAPI.h"

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace vk {
	bool IsDepth(VkFormat format);
	bool IsStencil(VkFormat format);

	void SetupMemoryProperties(VkPhysicalDevice physicalDevice);
	auto FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) ->uint32;

	auto CreateImage2D(VkDevice device, uint32 width, uint32 height, VkFormat format, VkImageUsageFlags usage, uint32 mipLevels)->VkImage;
	void DestroyImage(VkDevice device, VkImage image);

	auto CreateImageView(VkDevice device, VkImage image, VkFormat format, uint32 mipLevels)->VkImageView;
	void DestroyImageView(VkDevice device, VkImageView imageView);

	auto CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkSharingMode sharingMode)->VkBuffer;
	void DestroyBuffer(VkDevice device, VkBuffer buffer);

	auto AllocateMemory(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags memoryPropertyFlags)->VkDeviceMemory;
	auto AllocateMemory(VkDevice device, VkImage image, VkMemoryPropertyFlags memoryPropertyFlags)->VkDeviceMemory;
	void FreeMemory(VkDevice device, VkDeviceMemory memory);
	void SetData(VkDevice device, VkDeviceMemory memory, const void* data, VkDeviceSize range, VkDeviceSize offset = 0);

	void BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset = 0);
	void BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize offset = 0);

	auto CreateBufferView(VkDevice device, VkBuffer buffer, VkFormat format, VkDeviceSize range, VkDeviceSize offset = 0)->VkBufferView;
	void DestroyBufferView(VkDevice device, VkBufferView view);

	auto CreateFence(VkDevice device, VkFenceCreateFlags flags)->VkFence;
	void DestroyFence(VkDevice device, VkFence fence);

	auto CreateSemaphore(VkDevice device, VkSemaphoreCreateFlags flags)->VkSemaphore;
	void DestroySemaphore(VkDevice device, VkSemaphore semaphore);

	void CmdBegin(VkCommandBuffer commandBuffer);
	void CmdEnd(VkCommandBuffer commandBuffer);

	void CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer src, VkBuffer dst, VkDeviceSize range);
	void CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer src, VkImage dst, uint32 width, uint32 height);

	void CmdTransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
}