#include "VkHelper.h"
#include "VkAllocator.h"

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Memory.h>

namespace {
	VkPhysicalDeviceMemoryProperties g_PhysicalDeviceMemoryProperties;
}

bool vk::IsDepth(VkFormat format) {
	return
		format == VK_FORMAT_D16_UNORM ||
		format == VK_FORMAT_D16_UNORM_S8_UINT ||
		format == VK_FORMAT_D24_UNORM_S8_UINT ||
		format == VK_FORMAT_D32_SFLOAT ||
		format == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

bool vk::IsStencil(VkFormat format) {
	return
		format == VK_FORMAT_S8_UINT ||
		format == VK_FORMAT_D16_UNORM_S8_UINT ||
		format == VK_FORMAT_D24_UNORM_S8_UINT ||
		format == VK_FORMAT_D32_SFLOAT_S8_UINT;
}

void vk::SetupMemoryProperties(VkPhysicalDevice physicalDevice) {
	vk::API::GetPhysicalDeviceMemoryProperties(physicalDevice, &g_PhysicalDeviceMemoryProperties);
}

auto vk::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) -> uint32 {
	for (uint32 i = 0; i < g_PhysicalDeviceMemoryProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (g_PhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
			return i;
		}
	}

	DFAssert(false, "Can't find suitable memory type!");

	return 0;
}

auto vk::CreateImage2D(VkDevice device, uint32 width, uint32 height, VkFormat format, VkImageUsageFlags usage, uint32 mipLevels)->VkImage {
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = format;
	imageCreateInfo.extent = VkExtent3D({ width, height, 1 });
	imageCreateInfo.mipLevels = mipLevels;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.usage = usage;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkImage image = VK_NULL_HANDLE;
	if (vk::API::CreateImage(device, &imageCreateInfo, vk::Allocator(), &image) != VK_SUCCESS) {
		DFAssert(false, "Can't create Image!");
	}
	return image;
}

void vk::DestroyImage(VkDevice device, VkImage image) {
	vk::API::DestroyImage(device, image, vk::Allocator());
}

auto vk::CreateImageView(VkDevice device, VkImage image, VkFormat format, uint32 mipLevels)->VkImageView {
	const bool isDepth = IsDepth(format);
	const bool isStencil = IsStencil(format);

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.subresourceRange = {};
	imageViewCreateInfo.subresourceRange.aspectMask = (!isDepth && !isStencil) ? VK_IMAGE_ASPECT_COLOR_BIT : 0;
	imageViewCreateInfo.subresourceRange.aspectMask |= isDepth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
	imageViewCreateInfo.subresourceRange.aspectMask |= isStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = mipLevels;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	imageViewCreateInfo.image = image;

	VkImageView imageView = VK_NULL_HANDLE;
	if (vk::API::CreateImageView(device, &imageViewCreateInfo, vk::Allocator(), &imageView) != VK_SUCCESS) {
		DFAssert(false, "Can't create ImageView!");
	}
	return imageView;
}

void vk::DestroyImageView(VkDevice device, VkImageView imageView) {
	vk::API::DestroyImageView(device, imageView, vk::Allocator());
}

auto vk::CreateBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkSharingMode sharingMode) ->VkBuffer {
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = size;
	bufferCreateInfo.usage = bufferUsageFlags;
	bufferCreateInfo.sharingMode = sharingMode;

	VkBuffer buffer = VK_NULL_HANDLE;
	if (vk::API::CreateBuffer(device, &bufferCreateInfo, vk::Allocator(), &buffer) != VK_SUCCESS) {
		DFAssert(false, "Can't create Buffer!");
	}

	return buffer;
}

void vk::DestroyBuffer(VkDevice device, VkBuffer buffer) {
	vk::API::DestroyBuffer(device, buffer, vk::Allocator());
}

auto vk::AllocateMemory(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags memoryPropertyFlags)->VkDeviceMemory {
	VkMemoryRequirements memoryRequirements;
	vk::API::GetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.memoryTypeIndex = vk::FindMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags);
	memoryAllocateInfo.allocationSize = memoryRequirements.size;

	VkDeviceMemory memory = VK_NULL_HANDLE;
	if (vk::API::AllocateMemory(device, &memoryAllocateInfo, vk::Allocator(), &memory) != VK_SUCCESS) {
		DFAssert(false, "Can't allocate DeviceMemory!");
	}

	return memory;
}

auto vk::AllocateMemory(VkDevice device, VkImage image, VkMemoryPropertyFlags memoryPropertyFlags)->VkDeviceMemory {
	VkMemoryRequirements memoryRequirements;
	vk::API::GetImageMemoryRequirements(device, image, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.memoryTypeIndex = vk::FindMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags);
	memoryAllocateInfo.allocationSize = memoryRequirements.size;

	VkDeviceMemory memory = VK_NULL_HANDLE;
	if (vk::API::AllocateMemory(device, &memoryAllocateInfo, vk::Allocator(), &memory) != VK_SUCCESS) {
		DFAssert(false, "Can't allocate DeviceMemory!");
	}

	return memory;
}

void vk::FreeMemory(VkDevice device, VkDeviceMemory memory) {
	vk::API::FreeMemory(device, memory, vk::Allocator());
}

void vk::SetData(VkDevice device, VkDeviceMemory memory, const void* data, VkDeviceSize range, VkDeviceSize offset /*= 0*/) {
	void* mappedData = nullptr;
	vk::API::MapMemory(device, memory, offset, range, 0, &mappedData);
	df::MemCpy(mappedData, data, size_t(range));
	vk::API::UnmapMemory(device, memory);
}

void vk::BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize offset /*= 0*/) {
	DFAssert(device != VK_NULL_HANDLE, "Device is null!");
	DFAssert(buffer != VK_NULL_HANDLE, "Buffer is null!");
	DFAssert(memory != VK_NULL_HANDLE, "Memory is null!");

	if (vk::API::BindBufferMemory(device, buffer, memory, offset) != VK_SUCCESS) {
		DFAssert(false, "Can't bind BufferMemory!");
	}
}

void vk::BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize offset /*= 0*/) {
	DFAssert(device != VK_NULL_HANDLE, "Device is null!");
	DFAssert(image != VK_NULL_HANDLE, "Image is null!");
	DFAssert(memory != VK_NULL_HANDLE, "Memory is null!");

	if (vk::API::BindImageMemory(device, image, memory, offset) != VK_SUCCESS) {
		DFAssert(false, "Can't bind BufferMemory!");
	}
}

auto vk::CreateBufferView(VkDevice device, VkBuffer buffer, VkFormat format, VkDeviceSize range, VkDeviceSize offset /*= 0*/)->VkBufferView {
	VkBufferViewCreateInfo bufferViewCreateInfo = {};
	bufferViewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
	bufferViewCreateInfo.buffer = buffer;
	bufferViewCreateInfo.format = format;
	bufferViewCreateInfo.offset = offset;
	bufferViewCreateInfo.range = range;

	VkBufferView view = VK_NULL_HANDLE;
	if (vk::API::CreateBufferView(device, &bufferViewCreateInfo, vk::Allocator(), &view) != VK_SUCCESS) {
		DFAssert(false, "Can't create BufferView!");
	}

	return view;
}

void vk::DestroyBufferView(VkDevice device, VkBufferView view) {
	vk::API::DestroyBufferView(device, view, vk::Allocator());
}

auto vk::CreateFence(VkDevice device, VkFenceCreateFlags flags)->VkFence {
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = flags;

	VkFence fence = VK_NULL_HANDLE;
	if (vk::API::CreateFence(device, &fenceCreateInfo, vk::Allocator(), &fence) != VK_SUCCESS) {
		DFAssert(false, "Can't create Fence!");
	}

	return fence;
}

void vk::DestroyFence(VkDevice device, VkFence fence) {
	vk::API::DestroyFence(device, fence, vk::Allocator());
}

auto vk::CreateSemaphore(VkDevice device, VkSemaphoreCreateFlags flags)->VkSemaphore {
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.flags = flags;

	VkSemaphore semaphore = VK_NULL_HANDLE;
	if (vk::API::CreateSemaphore(device, &semaphoreCreateInfo, vk::Allocator(), &semaphore) != VK_SUCCESS) {
		DFAssert(false, "Can't create Semaphore!");
	}

	return semaphore;
}

void vk::DestroySemaphore(VkDevice device, VkSemaphore semaphore) {
	vk::API::DestroySemaphore(device, semaphore, vk::Allocator());
}

void vk::CmdBegin(VkCommandBuffer commandBuffer) {
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vk::API::BeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		DFAssert(false, "Can't begin recording CommandBuffer!");
	}
}

void vk::CmdEnd(VkCommandBuffer commandBuffer) {
	if (vk::API::EndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		DFAssert(false, "Failed to record CommandBuffer!");
	}
}

void vk::CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer src, VkBuffer dst, VkDeviceSize range) 	{
	VkBufferCopy copyRegion = {};
	copyRegion.size = range;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	vk::API::CmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);
}

void vk::CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer src, VkImage dst, uint32 width, uint32 height) {
	VkBufferImageCopy copyRegion = {};
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;
	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = { 0, 0, 0 };
	copyRegion.imageExtent = {width, height, 1};

	vk::API::CmdCopyBufferToImage(commandBuffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
}

void vk::CmdTransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else {
		DFAssert(false, "Unsupported layout transition!");
	}

	vk::API::CmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}