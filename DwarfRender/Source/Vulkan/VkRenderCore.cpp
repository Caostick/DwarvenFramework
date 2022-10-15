#include "VkInit.h"
#include "VkAllocator.h"
#include "VkDefinitions.h"
#include "VkDebug.h"
#include "VkHelper.h"
#include "VkTypeConversions.h"

#include "../RenderCore.h"
#include "../CommandBuffer.h"
#include "../DescriptorSetDefinitions.h"

#include <DwarfWindow/Window.h>

#include <DwarfRender/ParamSetDefinition.h>
#include <DwarfRender/GlobalObjects.h>

#include <DwarfResources/ResourceManager.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Memory.h>
#include <DwarvenCore/New.h>

namespace {

	auto ToVkPipelineRasterizationStateCreateInfo(const rf::RasterizationState* rasterizationState)->VkPipelineRasterizationStateCreateInfo {
		VkPipelineRasterizationStateCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthClampEnable = VK_FALSE;
		createInfo.rasterizerDiscardEnable = rasterizationState->m_RasterizerDiscardEnable;
		createInfo.polygonMode = rf::api::ToVk(rasterizationState->m_PolygonMode);
		createInfo.cullMode = rf::api::ToVk(rasterizationState->m_CullMode);
		createInfo.frontFace = rf::api::ToVk(rasterizationState->m_FrontFace);
		createInfo.depthBiasEnable = VK_FALSE;
		createInfo.depthBiasConstantFactor = 0.0f;
		createInfo.depthBiasClamp = 0.0f;
		createInfo.depthBiasSlopeFactor = 0.0f;
		createInfo.lineWidth = 1.0f;

		return createInfo;
	}

	void ToVkPipelineColorBlendStateCreateInfo(VkPipelineColorBlendStateCreateInfo& createInfo, df::Vector<VkPipelineColorBlendAttachmentState>& states, const rf::BlendState* blendState, uint32 count) {

		VkPipelineColorBlendAttachmentState attachmentState = {};
		attachmentState.blendEnable = blendState->m_BlendEnable;

		attachmentState.srcColorBlendFactor = rf::api::ToVk(blendState->m_SrcColorBlendFactor);
		attachmentState.dstColorBlendFactor = rf::api::ToVk(blendState->m_DstColorBlendFactor);
		attachmentState.colorBlendOp = rf::api::ToVk(blendState->m_ColorBlendOp);
		attachmentState.srcAlphaBlendFactor = rf::api::ToVk(blendState->m_SrcAlphaBlendFactor);
		attachmentState.dstAlphaBlendFactor = rf::api::ToVk(blendState->m_DstAlphaBlendFactor);
		attachmentState.alphaBlendOp = rf::api::ToVk(blendState->m_AlphaBlendOp);
		attachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		states = {};
		states.resize(count);

		for (auto& it : states) {
			it = attachmentState;
		}

		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.logicOpEnable = VK_FALSE;
		createInfo.logicOp = VK_LOGIC_OP_COPY;
		createInfo.attachmentCount = count;
		createInfo.pAttachments = states.data();
		createInfo.blendConstants[0] = 0.0f;
		createInfo.blendConstants[1] = 0.0f;
		createInfo.blendConstants[2] = 0.0f;
		createInfo.blendConstants[3] = 0.0f;
	}

	auto ToVkPipelineDepthStencilStateCreateInfo(const rf::DepthState* depthState) ->VkPipelineDepthStencilStateCreateInfo {
		VkPipelineDepthStencilStateCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.depthTestEnable = depthState->m_DepthTestEnable;
		createInfo.depthWriteEnable = depthState->m_DepthWriteEnable;
		createInfo.depthCompareOp = rf::api::ToVk(depthState->m_DepthCompareOp);
		createInfo.depthBoundsTestEnable = VK_FALSE;
		createInfo.stencilTestEnable = depthState->m_StencilTestEnable;
		createInfo.front = {};
		createInfo.back = {};
		createInfo.minDepthBounds = 0.0f;
		createInfo.maxDepthBounds = 1.0f;

		return createInfo;
	}

	auto GetStride(rf::EVertexAttributeFormat format) -> uint32 {
		switch (format) {
		case rf::EVertexAttributeFormat::X8_Norm:
			return 1;
		case rf::EVertexAttributeFormat::XY8_Norm:
			return 2;
		case rf::EVertexAttributeFormat::XYZ8_Norm:
			return 3;
		case rf::EVertexAttributeFormat::XYZW8_Norm:
			return 4;
		case rf::EVertexAttributeFormat::X16_Norm:
			return 2;
		case rf::EVertexAttributeFormat::XY16_Norm:
			return 4;
		case rf::EVertexAttributeFormat::XYZ16_Norm:
			return 6;
		case rf::EVertexAttributeFormat::XYZW16_Norm:
			return 8;
		case rf::EVertexAttributeFormat::X8_UNorm:
			return 1;
		case rf::EVertexAttributeFormat::XY8_UNorm:
			return 2;
		case rf::EVertexAttributeFormat::XYZ8_UNorm:
			return 3;
		case rf::EVertexAttributeFormat::XYZW8_UNorm:
			return 4;
		case rf::EVertexAttributeFormat::X16_UNorm:
			return 2;
		case rf::EVertexAttributeFormat::XY16_UNorm:
			return 4;
		case rf::EVertexAttributeFormat::XYZ16_UNorm:
			return 6;
		case rf::EVertexAttributeFormat::XYZW16_UNorm:
			return 6;
		case rf::EVertexAttributeFormat::X16_Float:
			return 2;
		case rf::EVertexAttributeFormat::XY16_Float:
			return 4;
		case rf::EVertexAttributeFormat::XYZ16_Float:
			return 6;
		case rf::EVertexAttributeFormat::XYZW16_Float:
			return 8;
		case rf::EVertexAttributeFormat::X32_Float:
			return 4;
		case rf::EVertexAttributeFormat::XY32_Float:
			return 8;
		case rf::EVertexAttributeFormat::XYZ32_Float:
			return 12;
		case rf::EVertexAttributeFormat::XYZW32_Float:
			return 16;
		case rf::EVertexAttributeFormat::X64_Float:
			return 8;
		case rf::EVertexAttributeFormat::XY64_Float:
			return 16;
		case rf::EVertexAttributeFormat::XYZ64_Float:
			return 24;
		case rf::EVertexAttributeFormat::XYZW64_Float:
			return 32;
		case rf::EVertexAttributeFormat::X8_Int:
			return 1;
		case rf::EVertexAttributeFormat::XY8_Int:
			return 2;
		case rf::EVertexAttributeFormat::XYZ8_Int:
			return 3;
		case rf::EVertexAttributeFormat::XYZW8_Int:
			return 4;
		case rf::EVertexAttributeFormat::X16_Int:
			return 2;
		case rf::EVertexAttributeFormat::XY16_Int:
			return 4;
		case rf::EVertexAttributeFormat::XYZ16_Int:
			return 6;
		case rf::EVertexAttributeFormat::XYZW16_Int:
			return 8;
		case rf::EVertexAttributeFormat::X32_Int:
			return 4;
		case rf::EVertexAttributeFormat::XY32_Int:
			return 8;
		case rf::EVertexAttributeFormat::XYZ32_Int:
			return 12;
		case rf::EVertexAttributeFormat::XYZW32_Int:
			return 16;
		case rf::EVertexAttributeFormat::X64_Int:
			return 8;
		case rf::EVertexAttributeFormat::XY64_Int:
			return 16;
		case rf::EVertexAttributeFormat::XYZ64_Int:
			return 24;
		case rf::EVertexAttributeFormat::XYZW64_Int:
			return 32;
		case rf::EVertexAttributeFormat::X8_UInt:
			return 1;
		case rf::EVertexAttributeFormat::XY8_UInt:
			return 2;
		case rf::EVertexAttributeFormat::XYZ8_UInt:
			return 3;
		case rf::EVertexAttributeFormat::XYZW8_UInt:
			return 4;
		case rf::EVertexAttributeFormat::X16_UInt:
			return 2;
		case rf::EVertexAttributeFormat::XY16_UInt:
			return 4;
		case rf::EVertexAttributeFormat::XYZ16_UInt:
			return 6;
		case rf::EVertexAttributeFormat::XYZW16_UInt:
			return 8;
		case rf::EVertexAttributeFormat::X32_UInt:
			return 4;
		case rf::EVertexAttributeFormat::XY32_UInt:
			return 8;
		case rf::EVertexAttributeFormat::XYZ32_UInt:
			return 12;
		case rf::EVertexAttributeFormat::XYZW32_UInt:
			return 16;
		case rf::EVertexAttributeFormat::X64_UInt:
			return 8;
		case rf::EVertexAttributeFormat::XY64_UInt:
			return 16;
		case rf::EVertexAttributeFormat::XYZ64_UInt:
			return 24;
		case rf::EVertexAttributeFormat::XYZW64_UInt:
			return 32;
		default:
			return 0;
		}
	}
}

rf::RenderCore::RenderCore() {
}

rf::RenderCore::~RenderCore() {
	m_RasterizationStates.Clear();
	m_BlendStates.Clear();
	m_DepthStates.Clear();
	m_VertexDescriptions.Clear();
}

auto rf::RenderCore::GetAPIData()->rf::api::RenderCore& {
	return m_APIData;
}

bool rf::RenderCore::Init(df::Window* window) {
	m_NumFramesInFlight = 3;

	m_FrameData.resize(m_NumFramesInFlight);
	m_FrameIndex = 0;

	if (!vk::API::Load()) {
		return false;
	}

	if (!InitInstance()) {
		return false;
	}

	if (!m_APIData.m_Presentation.CreateSurface(m_APIData.m_VkInstance, window)) {
		return false;
	}

	if (!InitPhysicalDevice()) {
		return false;
	}

	if (!CheckForPreventedCaptureSoft()) {
		return false;
	}

	if (!InitDevice()) {
		return false;
	}

	vk::API::GetDeviceQueue(m_APIData.m_VkDevice, m_APIData.m_PresentFamilyIndex, 0, &m_APIData.m_PresentQueue);
	vk::API::GetDeviceQueue(m_APIData.m_VkDevice, m_APIData.m_GraphicsFamilyIndex, 0, &m_APIData.m_GraphicsQueue);
	vk::API::GetDeviceQueue(m_APIData.m_VkDevice, m_APIData.m_TransferFamilyIndex, 0, &m_APIData.m_TransferQueue);
	vk::API::GetDeviceQueue(m_APIData.m_VkDevice, m_APIData.m_ComputeFamilyIndex, 0, &m_APIData.m_ComputeQueue);

	if (!m_APIData.m_Presentation.CreateSwapchain(
		m_APIData.m_VkDevice,
		m_APIData.m_VkPhysicalDevice,
		{ window->GetWidth(), window->GetHeight() },
		true,
		m_APIData.m_GraphicsFamilyIndex,
		m_APIData.m_PresentFamilyIndex
	)) {
		return false;
	}

	if (!InitCommandPools()) {
		return false;
	}

	if (!InitDescriptorPools()) {
		return false;
	}

	for (auto& frameData : m_FrameData) {
		if (!InitFrameData(frameData)) {
			return false;
		}
	}

	// Transfer
	m_TransferCommandBuffer.GetAPIData().Init("TransferCommandBuffer", m_APIData.m_VkDevice, m_APIData.m_TransferCommandPools[0]);

	m_APIData.m_TransferBuffer.Init(m_APIData.m_VkDevice, 1024 * 1024 * 32);

	return true;
}

void rf::RenderCore::Release() {
	m_APIData.m_TransferBuffer.Release(m_APIData.m_VulkanObjectManager);
	m_APIData.EmptyTrashCan();

	m_TransferCommandBuffer.GetAPIData().Release(m_APIData.m_VkDevice, m_APIData.m_TransferCommandPools[0]);

	for (auto& frameData : m_FrameData) {
		ReleaseFrameData(frameData);
	}
	m_FrameData.clear();

	ReleaseDescriptorPools();

	ReleaseCommandPools();

	// Destroy swapchain
	m_APIData.m_Presentation.DestroySwapchain(m_APIData.m_VkDevice);

	m_APIData.EmptyTrashCan();

	// Destroy device and physical device
	DFAssert(m_APIData.m_VkDevice != VK_NULL_HANDLE, "Device not created!");
	vk::API::DestroyDevice(m_APIData.m_VkDevice, vk::Allocator());
	m_APIData.m_VkDevice = VK_NULL_HANDLE;
	m_APIData.m_VkPhysicalDevice = VK_NULL_HANDLE;

	// Destroy surface
	m_APIData.m_Presentation.DestroySurface(m_APIData.m_VkInstance);

	// Destroy instance
	DFAssert(m_APIData.m_VkInstance != VK_NULL_HANDLE, "Instance not created!");
	vk::DestroyDebugCallback(m_APIData.m_VkInstance);
	vk::API::DestroyInstance(m_APIData.m_VkInstance, vk::Allocator());
	m_APIData.m_VkInstance = VK_NULL_HANDLE;
}

bool rf::RenderCore::Load() {
	bool result = true;

	result &= m_APIData.m_Presentation.Load(*this);

	return result;
}

void rf::RenderCore::Unload() {
	m_APIData.m_Presentation.Unload(*this);
}

bool rf::RenderCore::BeginFrame(rf::RenderContext& renderContext) {

	m_APIData.m_VulkanObjectManager.Update();

	rf::FrameData& frame = m_FrameData[m_FrameIndex];

	vk::API::WaitForFences(m_APIData.m_VkDevice, 1, &frame.m_APIData.m_InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());

	const VkResult result = m_APIData.m_Presentation.AquireNextImage(m_APIData.m_VkDevice, frame.m_APIData.m_ImageAvailableSemaphore);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		CompleteAllCommands();
		RecreateSwapchain();
		return false;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		DFAssert(false, "Failed to acquire swap chain image!");
		return false;
	}


	vk::API::ResetFences(m_APIData.m_VkDevice, 1, &frame.m_APIData.m_InFlightFence);

	frame.m_CommandBuffer.Begin();

	for (auto buffer : m_Buffers) {
		if (!buffer->m_IsStatic) {
			SyncBufferData(buffer, m_FrameIndex);
		}
	}

	// Transfer
	{
		DFScopedRenderEvent(frame.m_CommandBuffer, "Transfer");

		m_APIData.m_TransferBuffer.Execute(frame.m_CommandBuffer);
	}

	renderContext.m_CommandBuffer = &frame.m_CommandBuffer;
	renderContext.m_FrameIndex = m_FrameIndex;

	return true;
}

void rf::RenderCore::EndFrame() {

	rf::FrameData& frame = m_FrameData[m_FrameIndex];

	frame.m_CommandBuffer.End();

	const VkSemaphore waitSemaphores[] = { frame.m_APIData.m_ImageAvailableSemaphore };
	const VkSemaphore signalSemaphores[] = { frame.m_APIData.m_RenderFinishedSemaphore };
	const VkCommandBuffer commandBuffer = frame.m_CommandBuffer.GetAPIData().Get();
	const VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vk::API::QueueSubmit(m_APIData.m_GraphicsQueue, 1, &submitInfo, frame.m_APIData.m_InFlightFence) != VK_SUCCESS) {
		DFAssert(false, "Failed to submit render command buffer!");
	}

	const VkResult result = m_APIData.m_Presentation.Present(frame.m_APIData.m_RenderFinishedSemaphore, m_APIData.m_PresentQueue);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		CompleteAllCommands();
		RecreateSwapchain();
	} else if (result != VK_SUCCESS) {
		DFAssert(false, "Failed to present swap chain image!");
	}

	m_FrameIndex = (m_FrameIndex + 1) % m_NumFramesInFlight;
}

void rf::RenderCore::Present(rf::TextureId texture) {
	rf::FrameData& frameData = m_FrameData[m_FrameIndex];

	m_APIData.m_Presentation.PresentTexture(frameData.m_CommandBuffer, texture);
}

void rf::RenderCore::CompleteAllCommands() {
	for (int i = 0; i < m_FrameData.size(); ++i) {
		vk::API::WaitForFences(m_APIData.m_VkDevice, 1, &m_FrameData[i].m_APIData.m_InFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	}

	m_APIData.EmptyTrashCan();
}

bool rf::RenderCore::RecreateSwapchain(uint32 screenWidth, uint32 screenHeight) {
	return m_APIData.m_Presentation.RecreateSwapchain(
		*this,
		m_APIData.m_VkDevice,
		m_APIData.m_VkPhysicalDevice,
		VkExtent2D({ screenWidth, screenHeight }),
		true,
		m_APIData.m_GraphicsFamilyIndex,
		m_APIData.m_PresentFamilyIndex
	);
}

bool rf::RenderCore::RecreateSwapchain() {
	return m_APIData.m_Presentation.RecreateSwapchain(
		*this,
		m_APIData.m_VkDevice, 
		m_APIData.m_VkPhysicalDevice, 
		m_APIData.m_GraphicsFamilyIndex,
		m_APIData.m_PresentFamilyIndex
	);
}

auto rf::RenderCore::GetNumFramesInFlight()const->uint32 {
	return m_NumFramesInFlight;
}

auto rf::RenderCore::GetBufferAlignment(const BufferUsageFlags& usage)const->uint32 {
	uint32 alignment = 0;

	if (usage.Has(EBufferUsageFlag::Storage)) {
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minStorageBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::Uniform)) {
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minUniformBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::Vertex)) {
		alignment = std::max(alignment, uint32(sizeof(float)));
	}

	if (usage.Has(EBufferUsageFlag::Index)) {
		alignment = std::max(alignment, uint32(sizeof(uint32)));
	}

	if (usage.Has(EBufferUsageFlag::Indirect)) {
		alignment = std::max(alignment, uint32(sizeof(uint32)));
	}

	if (usage.Has(EBufferUsageFlag::StorageTexel)) {
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minStorageBufferOffsetAlignment));
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minTexelBufferOffsetAlignment));
	}

	if (usage.Has(EBufferUsageFlag::UniformTexel)) {
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minUniformBufferOffsetAlignment));
		alignment = std::max(alignment, uint32(m_APIData.m_DeviceLimits.minTexelBufferOffsetAlignment));
	}



	return alignment;
}

auto rf::RenderCore::RequestRasterizationState(const rf::RasterizationState& rasterizationState) -> rf::RasterizationStateId {
	for (auto ptr : m_RasterizationStates) {
		if (*ptr == rasterizationState) {
			return ptr;
		}
	}

	return m_RasterizationStates.Create(rasterizationState);
}

auto rf::RenderCore::RequestBlendState(const rf::BlendState& blendState) -> rf::BlendStateId {
	for (auto ptr : m_BlendStates) {
		if (*ptr == blendState) {
			return ptr;
		}
	}

	return m_BlendStates.Create(blendState);
}

auto rf::RenderCore::RequestDepthState(const rf::DepthState& depthState) -> rf::DepthStateId {
	for (auto ptr : m_DepthStates) {
		if (*ptr == depthState) {
			return ptr;
		}
	}

	return m_DepthStates.Create(depthState);
}

auto rf::RenderCore::RequestEmptyVertexDescription()->rf::VertexDescriptionId {
	return m_VertexDescriptions.Create();
}

auto rf::RenderCore::CreateSampler(const rf::SamplerState& samplerState)->rf::SamplerId {
	rf::Sampler* sampler = m_Samplers.Create();

	m_APIData.InitSampler(sampler, "", samplerState);

	return sampler;
}

void rf::RenderCore::DestroySampler(rf::SamplerId sampler) {
	m_APIData.ReleaseSampler(sampler);

	m_Samplers.Destroy(sampler);
}

auto rf::RenderCore::CreateBuffer(
	const df::StringView& name,
	uint32 size,
	rf::BufferUsageFlags usageFlags,
	rf::EBufferAccessType accessType,
	bool isStatic)->rf::BufferId {

	rf::Buffer* buffer = m_Buffers.Create();

	VkBufferUsageFlags bufferUsageFlags = api::ToVk(usageFlags);
	VkMemoryPropertyFlags memoryPropertyFlags = 0;

	switch (accessType) {
	case rf::EBufferAccessType::Transfer:
		bufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		memoryPropertyFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		break;
	case rf::EBufferAccessType::Direct:
		memoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		break;
	default:
		break;
	}

	const uint32 alignment = GetBufferAlignment(usageFlags);
	const uint32 count = size / alignment;
	const uint32 dataStride = (alignment * count == size) ? size : (alignment * (count + 1));

	buffer->m_AccessType = accessType;
	buffer->m_Size = size;
	buffer->m_Stride = dataStride;
	buffer->m_UpdateSize = size;
	buffer->m_UsageFlags = usageFlags;
	buffer->m_IsStatic = isStatic;
	buffer->m_Data = DFNew uint8[size];

	buffer->m_APIData.m_BufferSize = dataStride * (isStatic ? 1 : m_NumFramesInFlight);
	buffer->m_APIData.m_BufferUsageFlags = bufferUsageFlags;
	buffer->m_APIData.m_MemoryPropertyFlags = memoryPropertyFlags;

	buffer->m_APIData.m_Buffer = vk::CreateBuffer(
		m_APIData.m_VkDevice,
		buffer->m_APIData.m_BufferSize,
		buffer->m_APIData.m_BufferUsageFlags,
		VK_SHARING_MODE_EXCLUSIVE
	);
	DFVkDebugName(m_APIData.m_VkDevice, buffer->m_APIData.m_Buffer, name);

	buffer->m_APIData.m_Memory = vk::AllocateMemory(
		m_APIData.m_VkDevice,
		buffer->m_APIData.m_Buffer,
		buffer->m_APIData.m_MemoryPropertyFlags
	);
	DFVkDebugName(m_APIData.m_VkDevice, buffer->m_APIData.m_Memory, name);

	vk::BindBufferMemory(
		m_APIData.m_VkDevice,
		buffer->m_APIData.m_Buffer,
		buffer->m_APIData.m_Memory
	);

	return buffer;
}

void rf::RenderCore::SetBufferData(rf::BufferId buffer, const void* data, uint32 dataSize, uint32 offset /*= 0*/) {
	if (buffer->m_IsStatic) {
		const VkMemoryPropertyFlags memoryPropertyFlags = buffer->m_APIData.m_MemoryPropertyFlags;
		if (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {

			m_APIData.m_TransferBuffer.SetBufferData(
				buffer->m_APIData.m_Buffer,
				data,
				dataSize,
				offset
			);

		} else if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
			vk::SetData(
				m_APIData.m_VkDevice,
				buffer->m_APIData.m_Memory,
				data,
				dataSize,
				offset
			);
		} else {
			DFAssert(false, "Can't set buffer data!");
		}
	} else {
		df::MemCpy(buffer->m_Data + offset, data, dataSize);
	}
}

void rf::RenderCore::SyncBufferData(rf::BufferId buffer, uint32 frameIdx) {
	DFAssert(!buffer->m_IsStatic, "Static buffer shouldn't be in sync logic!");

	const auto data = buffer->m_Data;
	const auto dataSize = buffer->m_UpdateSize;
	const auto offset = buffer->m_Stride * frameIdx;

	const VkMemoryPropertyFlags memoryPropertyFlags = buffer->m_APIData.m_MemoryPropertyFlags;
	if (memoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {

		m_APIData.m_TransferBuffer.SetBufferData(
			buffer->m_APIData.m_Buffer,
			data,
			dataSize,
			offset
		);

	} else if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) && (memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
		vk::SetData(
			m_APIData.m_VkDevice,
			buffer->m_APIData.m_Memory,
			data,
			dataSize,
			offset
		);
	} else {
		DFAssert(false, "Can't set buffer data!");
	}
}

void rf::RenderCore::DestroyBuffer(rf::BufferId buffer) {
	m_APIData.m_VulkanObjectManager.RemoveDeviceMemory(m_APIData.m_VkDevice, buffer->m_APIData.m_Memory);
	m_APIData.m_VulkanObjectManager.RemoveBuffer(m_APIData.m_VkDevice, buffer->m_APIData.m_Buffer);

	DFDelete[]  buffer->m_Data;

	m_Buffers.Destroy(buffer);
}

auto rf::RenderCore::CreateTexture(
	const df::StringView& name,
	uint32 width,
	uint32 height,
	rf::ImageUsageFlags usage,
	rf::ETextureFormat format)->rf::TextureId {

	rf::Texture* texture = m_Textures.Create();
	texture->m_UsageFlags = usage;
	texture->m_Format = format;

	VkImageUsageFlags usageFlags = api::ToVk(usage);

	texture->m_Width = width;
	texture->m_Height = height;
	texture->m_APIData.m_Format = api::ToVk(format);

	texture->m_APIData.m_Image = vk::CreateImage2D(m_APIData.m_VkDevice, texture->m_Width, texture->m_Height, texture->m_APIData.m_Format, usageFlags, 1);
	DFVkDebugName(m_APIData.m_VkDevice, texture->m_APIData.m_Image, name);

	texture->m_APIData.m_Memory = vk::AllocateMemory(m_APIData.m_VkDevice, texture->m_APIData.m_Image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	DFVkDebugName(m_APIData.m_VkDevice, texture->m_APIData.m_Memory, name);

	vk::BindImageMemory(m_APIData.m_VkDevice, texture->m_APIData.m_Image, texture->m_APIData.m_Memory);
	texture->m_APIData.m_ImageView = vk::CreateImageView(m_APIData.m_VkDevice, texture->m_APIData.m_Image, texture->m_APIData.m_Format, 1);
	DFVkDebugName(m_APIData.m_VkDevice, texture->m_APIData.m_ImageView, name);

	texture->m_Name = name;

	return texture;
}

void rf::RenderCore::SetTextureData(rf::TextureId texture, void* data, uint32 dataSize, uint32 offset /*= 0*/) {
	rf::CommandBuffer& rcb = m_TransferCommandBuffer;

	VkBuffer tmpBuffer = vk::CreateBuffer(m_APIData.m_VkDevice, VkDeviceSize(dataSize), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
	VkDeviceMemory tmpMemory = vk::AllocateMemory(m_APIData.m_VkDevice, tmpBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	vk::BindBufferMemory(m_APIData.m_VkDevice, tmpBuffer, tmpMemory);
	vk::SetData(m_APIData.m_VkDevice, tmpMemory, data, dataSize, offset);

	rcb.Begin();
	rcb.TransitionImageLayout(texture, EImageLayout::Undefined, EImageLayout::TransferDst);
	rcb.GetAPIData().CopyBufferToImage(tmpBuffer, texture->m_APIData.m_Image, texture->m_Width, texture->m_Height);
	rcb.TransitionImageLayout(texture, EImageLayout::TransferDst, EImageLayout::ColorReadOnly);
	rcb.End();

	rcb.GetAPIData().Submit(m_APIData.m_TransferQueue);
	rcb.GetAPIData().Wait(m_APIData.m_TransferQueue);

	m_APIData.m_VulkanObjectManager.RemoveBuffer(m_APIData.m_VkDevice, tmpBuffer);
	m_APIData.m_VulkanObjectManager.RemoveDeviceMemory(m_APIData.m_VkDevice, tmpMemory);
}

void rf::RenderCore::DestroyTexture(rf::TextureId texture) {
	m_APIData.m_VulkanObjectManager.RemoveImage(m_APIData.m_VkDevice, texture->m_APIData.m_Image);
	m_APIData.m_VulkanObjectManager.RemoveImageView(m_APIData.m_VkDevice, texture->m_APIData.m_ImageView);
	m_APIData.m_VulkanObjectManager.RemoveDeviceMemory(m_APIData.m_VkDevice, texture->m_APIData.m_Memory);

	m_Textures.Destroy(texture);
}

auto rf::RenderCore::CreateMesh(uint32 vertexCount, uint32 indexCount)->MeshId {
	rf::Mesh* mesh = m_Meshes.Create(vertexCount, indexCount);

	return mesh;
}

void rf::RenderCore::SetMeshAttributeBuffer(MeshId mesh, uint32 attributeId, const void* /*data*/) {
	[[maybe_unused]] const uint32 attrSize = rf::VertexAttributeRegistry::Entries[attributeId].SizeOf;
	[[maybe_unused]] const uint32 dataSize = mesh->GetVertexCount() * attrSize;
}

void rf::RenderCore::SetMeshIndexBuffer(MeshId /*mesh*/, const uint32* /*data*/) {
	
}

void rf::RenderCore::DestroyMesh(MeshId mesh) {


	m_Meshes.Destroy(mesh);
}

auto rf::RenderCore::CreateRenderPass(
	const df::StringView& name,
	const df::Vector<rf::AttachmentDefinition>& attachments)->rf::PassId {
	rf::Pass* renderPass = m_RenderPasses.Create();

	const uint32 attachmentCount = uint32(attachments.size());

	df::Vector<VkAttachmentDescription> attachementDescriptions;
	attachementDescriptions.resize(attachments.size());

	df::Vector<rf::ClearValue> clearValues;
	clearValues.resize(attachments.size());

	for (uint32 i = 0; i < attachmentCount; ++i) {
		const rf::AttachmentDefinition& srcDef = attachments[i];

		VkAttachmentDescription& desc = attachementDescriptions[i];
		desc = {};
		desc.format = api::ToVk(srcDef.m_Format);
		desc.samples = VK_SAMPLE_COUNT_1_BIT;
		desc.loadOp = api::ToVk(srcDef.m_LoadOp);
		desc.storeOp = api::ToVk(srcDef.m_StoreOp);
		desc.stencilLoadOp = api::ToVk(srcDef.m_StencilLoadOp);
		desc.stencilStoreOp = api::ToVk(srcDef.m_StencilStoreOp);
		desc.initialLayout = api::ToVk(srcDef.m_InitialLayout);
		desc.finalLayout = api::ToVk(srcDef.m_FinalLayout);

		clearValues[i] = srcDef.m_ClearValue;
	}

	m_APIData.InitRenderPass(renderPass, name, attachementDescriptions, clearValues);

	return renderPass;
}

void rf::RenderCore::DestroyRenderPass(rf::PassId renderPass) {
	m_APIData.ReleaseRenderPass(renderPass);

	m_RenderPasses.Destroy(renderPass);
}

auto rf::RenderCore::CreateDescriptorSetLayout(
	const df::StringView& name,
	const df::Vector<rf::DescriptorDefinition>& descriptors)->rf::DescriptorSetLayoutId {

	rf::DescriptorSetLayout* descriptorSetLayout = m_DescriptorSetLayouts.Create();

	const uint32 descriptorCount = uint32(descriptors.size());

	df::Vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;
	descriptorSetLayoutBindings.resize(descriptorCount);

	for (uint32 i = 0; i < descriptorCount; ++i) {
		const rf::DescriptorDefinition& descriptor = descriptors[i];
		VkDescriptorSetLayoutBinding& binding = descriptorSetLayoutBindings[i];
		binding = {};
		binding.binding = descriptor.m_BindingIndex;
		binding.descriptorType = api::ToVk(descriptor.m_Type);
		binding.stageFlags = api::ToVk(descriptor.m_ShaderStageFlags);
		binding.descriptorCount = descriptor.m_Count;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = descriptorCount;
	layoutInfo.pBindings = descriptorSetLayoutBindings.data();

	if (vk::API::CreateDescriptorSetLayout(m_APIData.m_VkDevice, &layoutInfo, vk::Allocator(), &descriptorSetLayout->m_Handle) != VK_SUCCESS) {
		DFAssert(false, "Can't create DescriptorSetLayout!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, descriptorSetLayout->m_Handle, name);

	return descriptorSetLayout;
}

auto rf::RenderCore::CreateDescriptorSetLayout(
	const df::StringView& name,
	const rf::DescriptorLayout& vsDescriptorLayout)->rf::DescriptorSetLayoutId {

	df::Vector<rf::DescriptorDefinition> descriptors;

	for (uint32 i = 0; i < vk::Constants::MaxDescriptorCount; ++i) {
		if (vsDescriptorLayout.m_Descriptors[i] != rf::EDescriptorType::None) {
			rf::DescriptorDefinition descriptorDefinition;
			descriptorDefinition.m_BindingIndex = i;
			descriptorDefinition.m_Count = 1;
			descriptorDefinition.m_Type = vsDescriptorLayout.m_Descriptors[i];
			descriptorDefinition.m_ShaderStageFlags |= rf::EShaderStage::Vertex;

			descriptors.emplace_back(descriptorDefinition);
		}
	}

	return CreateDescriptorSetLayout(name, descriptors);
}

auto rf::RenderCore::CreateDescriptorSetLayout(
	const df::StringView& name,
	const rf::DescriptorLayout& vsDescriptorLayout,
	const rf::DescriptorLayout& fsDescriptorLayout)->rf::DescriptorSetLayoutId {

	df::Vector<rf::DescriptorDefinition> descriptors;

	for (uint32 i = 0; i < vk::Constants::MaxDescriptorCount; ++i) {
		const bool compatible = (
			(vsDescriptorLayout.m_Descriptors[i] == rf::EDescriptorType::None) ||
			(fsDescriptorLayout.m_Descriptors[i] == rf::EDescriptorType::None)) ||
			(vsDescriptorLayout.m_Descriptors[i] == fsDescriptorLayout.m_Descriptors[i]);

		DFAssert(compatible, "Vertex shader descriptor incompatible to fragment shader one!");
		if (!compatible) {
			continue;
		}

		const bool isUsed = (
			(vsDescriptorLayout.m_Descriptors[i] != rf::EDescriptorType::None) ||
			(fsDescriptorLayout.m_Descriptors[i] != rf::EDescriptorType::None));

		if (isUsed) {
			rf::DescriptorDefinition descriptorDefinition;
			descriptorDefinition.m_BindingIndex = i;
			descriptorDefinition.m_Count = 1;

			if (vsDescriptorLayout.m_Descriptors[i] != rf::EDescriptorType::None) {
				descriptorDefinition.m_Type = vsDescriptorLayout.m_Descriptors[i];
				descriptorDefinition.m_ShaderStageFlags |= rf::EShaderStage::Vertex;
			}

			if (fsDescriptorLayout.m_Descriptors[i] != rf::EDescriptorType::None) {
				descriptorDefinition.m_Type = fsDescriptorLayout.m_Descriptors[i];
				descriptorDefinition.m_ShaderStageFlags |= rf::EShaderStage::Fragment;
			}

			descriptors.emplace_back(descriptorDefinition);
		}
	}

	return CreateDescriptorSetLayout(name, descriptors);
}

void rf::RenderCore::DestroyDescriptorSetLayout(rf::DescriptorSetLayoutId descriptorSetLayout) {
	m_APIData.m_VulkanObjectManager.RemoveDescriptorSetLayout(m_APIData.m_VkDevice, descriptorSetLayout->m_Handle);

	m_DescriptorSetLayouts.Destroy(descriptorSetLayout);
}


auto rf::RenderCore::CreateDescriptorSet(const df::StringView& name, uint32 id, rf::DescriptorSetLayoutId descriptorSetLayout)->rf::DescriptorSetId {
	rf::DescriptorSet* descriptorSet = m_DescriptorSets.Create();
	descriptorSet->m_APIData.m_Device = m_APIData.m_VkDevice;
	descriptorSet->m_Id = id;

	VkDescriptorSetAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool = m_APIData.m_DescriptorPool;
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &descriptorSetLayout->m_Handle;

	vk::API::AllocateDescriptorSets(m_APIData.m_VkDevice, &allocateInfo, &descriptorSet->m_APIData.m_Handle);

	DFVkDebugName(m_APIData.m_VkDevice, descriptorSet->m_APIData.m_Handle, name);

	return descriptorSet;
}

void rf::RenderCore::DestroyDescriptorSet(rf::DescriptorSetId descriptorSet) {
	m_APIData.m_VulkanObjectManager.RemoveDescriptorSet(m_APIData.m_VkDevice, m_APIData.m_DescriptorPool, descriptorSet->m_APIData.m_Handle);

	m_DescriptorSets.Destroy(descriptorSet);
}

auto rf::RenderCore::CreatePipelineLayout(const df::StringView& name, const df::Vector<rf::DescriptorSetLayoutId>& descriptorSets)->rf::PipelineLayoutId {
	rf::PipelineLayout* pipelineLayout = m_PipelineLayouts.Create();

	const uint32 descriptorSetLayoutCount = uint32(descriptorSets.size());

	df::Vector<VkDescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.resize(descriptorSetLayoutCount);

	for (uint32 i = 0; i < descriptorSetLayoutCount; ++i) {
		descriptorSetLayouts[i] = descriptorSets[i]->m_Handle;
	}

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayoutCount;
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vk::API::CreatePipelineLayout(m_APIData.m_VkDevice, &pipelineLayoutInfo, vk::Allocator(), &pipelineLayout->m_Handle) != VK_SUCCESS) {
		DFAssert(false, "Can't create PipelineLayout!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, pipelineLayout->m_Handle, name);

	return pipelineLayout;
}

void rf::RenderCore::DestroyPipelineLayout(rf::PipelineLayoutId pipelineLayout) {
	m_APIData.m_VulkanObjectManager.RemovePipelineLayout(m_APIData.m_VkDevice, pipelineLayout->m_Handle);

	m_PipelineLayouts.Destroy(pipelineLayout);
}

auto rf::RenderCore::CreateFramebuffer(const df::StringView& name, rf::PassId renderPass, const df::Vector<rf::TextureId>& renderTargets)->rf::FramebufferId {
	const uint32 attachmentCount = uint32(renderTargets.size());

	df::Vector<VkImageView> attachments;
	attachments.resize(attachmentCount);

	uint32 width = 0;
	uint32 height = 0;

	for (uint32 i = 0; i < attachmentCount; ++i) {
		const rf::Texture& rt = *renderTargets[i];

		attachments[i] = rt.m_APIData.m_ImageView;
		if (rt.m_Width != width || rt.m_Height != height) {
			DFAssert(width == 0 && height == 0, "Framebuffer target sized are different!");

			width = rt.m_Width;
			height = rt.m_Height;
		}
	}

	rf::Framebuffer* framebuffer = m_Framebuffers.Create();
	m_APIData.InitFramebuffer(framebuffer, name, renderPass, attachments, width, height);

	return framebuffer;
}

void rf::RenderCore::DestroyFramebuffer(rf::FramebufferId framebuffer) {
	m_APIData.ReleaseFramebuffer(framebuffer);

	m_Framebuffers.Destroy(framebuffer);
}


auto rf::RenderCore::CreateShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ShaderModuleId {
	return CreateShaderModule(name, code.data(), uint32(code.size()));
}

auto rf::RenderCore::CreateShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ShaderModuleId {
	rf::ShaderModule* shaderModule = m_ShaderModules.Create();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = codeLength * sizeof(uint32);
	shaderModuleCreateInfo.pCode = codeData;

	if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &(shaderModule->m_APIData.m_Handle)) != VK_SUCCESS) {
		DFAssert(false, "Can't create ShaderModule!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle, name);

	return shaderModule;
}

void rf::RenderCore::DestroyShaderModule(rf::ShaderModuleId shaderModule) {
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle);
	m_ShaderModules.Destroy(shaderModule);
}

auto rf::RenderCore::CreateVertexShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::VertexShaderModuleId {
	return CreateVertexShaderModule(name, code.data(), uint32(code.size()));
}

auto rf::RenderCore::CreateVertexShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::VertexShaderModuleId {
	rf::VertexShaderModule* shaderModule = m_VertexShaderModules.Create();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = codeLength * sizeof(uint32);
	shaderModuleCreateInfo.pCode = codeData;

	if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &(shaderModule->m_APIData.m_Handle)) != VK_SUCCESS) {
		DFAssert(false, "Can't create VertexShaderModule!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle, name);

	return shaderModule;
}

void rf::RenderCore::DestroyVertexShaderModule(rf::VertexShaderModuleId shaderModule) {
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle);
	m_VertexShaderModules.Destroy(shaderModule);
}

auto rf::RenderCore::CreateFragmentShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::FragmentShaderModuleId {
	return CreateFragmentShaderModule(name, code.data(), uint32(code.size()));
}

auto rf::RenderCore::CreateFragmentShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::FragmentShaderModuleId {
	rf::FragmentShaderModule* shaderModule = m_FragmentShaderModules.Create();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = codeLength * sizeof(uint32);
	shaderModuleCreateInfo.pCode = codeData;

	if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &(shaderModule->m_APIData.m_Handle)) != VK_SUCCESS) {
		DFAssert(false, "Can't create FragmentShaderModule!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle, name);

	return shaderModule;
}

void rf::RenderCore::DestroyFragmentShaderModule(rf::FragmentShaderModuleId shaderModule) {
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle);
	m_FragmentShaderModules.Destroy(shaderModule);
}

auto rf::RenderCore::CreateComputeShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ComputeShaderModuleId {
	return CreateComputeShaderModule(name, code.data(), uint32(code.size()));
}

auto rf::RenderCore::CreateComputeShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ComputeShaderModuleId {
	rf::ComputeShaderModule* shaderModule = m_ComputeShaderModules.Create();

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = codeLength * sizeof(uint32);
	shaderModuleCreateInfo.pCode = codeData;

	if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &(shaderModule->m_APIData.m_Handle)) != VK_SUCCESS) {
		DFAssert(false, "Can't create ComputeShaderModule!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle, name);

	return shaderModule;
}

void rf::RenderCore::DestroyComputeShaderModule(rf::ComputeShaderModuleId shaderModule) {
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, shaderModule->m_APIData.m_Handle);
	m_ComputeShaderModules.Destroy(shaderModule);
}

auto rf::RenderCore::CreatePipelineStateObject(const df::StringView& name, const rf::PipelineDescriptor& pipelineDescriptor)->rf::PipelineId {
	rf::Pipeline* pipeline = m_Pipelines.Create();

	const char* shaderEntryPoint = "main";

	df::Vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
	if (pipelineDescriptor.m_VertexShaderId != nullptr) {
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		pipelineShaderStageCreateInfo.module = pipelineDescriptor.m_VertexShaderId->m_APIData.m_Handle;
		pipelineShaderStageCreateInfo.pName = shaderEntryPoint;

		shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
	}
	if (pipelineDescriptor.m_FragmentShaderId != nullptr) {
		VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		pipelineShaderStageCreateInfo.module = pipelineDescriptor.m_FragmentShaderId->m_APIData.m_Handle;
		pipelineShaderStageCreateInfo.pName = shaderEntryPoint;

		shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
	}

	df::Vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	const VkPipelineLayout& vkPipelineLayout = pipelineDescriptor.m_PipelineLayoutId->m_Handle;
	const rf::Pass* renderPass = pipelineDescriptor.m_RenderPassId;
	const uint32 colorAttachmentCount = renderPass->m_APIData.m_ColorAttachmentCount;

	const rf::BlendStateId blendStateId = pipelineDescriptor.m_BlendStateId;
	VkPipelineColorBlendStateCreateInfo blendStateCreateInfo;
	df::Vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates;
	ToVkPipelineColorBlendStateCreateInfo(blendStateCreateInfo, blendAttachmentStates, blendStateId, colorAttachmentCount);

	const auto& vertexDescription = pipelineDescriptor.m_VertexDescriptionId;
	const auto& vertexInputBindingDescriptions = vertexDescription->m_APIData.GetBindingDescriptions();
	const auto& vertexInputAttributeDescriptions = vertexDescription->m_APIData.GetAttributeDescriptions();

	const rf::DepthStateId depthStateId = pipelineDescriptor.m_DepthStateId;
	const VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = ToVkPipelineDepthStencilStateCreateInfo(depthStateId);

	const rf::RasterizationStateId rasterizationStateId = pipelineDescriptor.m_RasterizationStateId;
	const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = ToVkPipelineRasterizationStateCreateInfo(rasterizationStateId);

	const uint32 defaultWidth = 4;
	const uint32 defaultHidth = 4;

	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexDescription->m_APIData.GetBindingDescriptionCount();
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexDescription->m_APIData.GetAttributeDescriptionCount();
	vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions;
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = api::ToVk(pipelineDescriptor.m_PrimitiveTopology);
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = float(defaultWidth);
	viewport.height = float(defaultHidth);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent.width = defaultWidth;
	scissor.extent.height = defaultHidth;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
	multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = uint32(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = uint32(shaderStageCreateInfos.size());
	graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.data();
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &blendStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = vkPipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass->m_APIData.m_RenderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vk::API::CreateGraphicsPipelines(m_APIData.m_VkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, vk::Allocator(), &pipeline->m_Handle) != VK_SUCCESS) {
		DFAssert(false, "Can't create GraphicsPipeline!");
	}

	DFVkDebugName(m_APIData.m_VkDevice, pipeline->m_Handle, name);

	return pipeline;
}

void rf::RenderCore::DestroyPipelineStateObject(rf::PipelineId pipeline) {
	m_APIData.m_VulkanObjectManager.RemovePipeline(m_APIData.m_VkDevice, pipeline->m_Handle);

	m_Pipelines.Destroy(pipeline);
}

auto rf::RenderCore::CreateGraphicsPipeline()->rf::GraphicsPipelineId {
	return m_GraphicsPipelines.Create();
}

void rf::RenderCore::DestroyGraphicsPipeline(rf::GraphicsPipelineId pipeline) {
	for (auto&& instance : pipeline->m_APIData.m_Instances) {
		m_APIData.m_VulkanObjectManager.RemovePipeline(m_APIData.m_VkDevice, instance.m_Pipeline);
	}
	m_APIData.m_VulkanObjectManager.RemovePipelineLayout(m_APIData.m_VkDevice, pipeline->m_APIData.m_Layout);
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, pipeline->m_APIData.m_VSModule);
	m_APIData.m_VulkanObjectManager.RemoveShaderModule(m_APIData.m_VkDevice, pipeline->m_APIData.m_FSModule);

	m_GraphicsPipelines.Destroy(pipeline);
}

auto rf::RenderCore::CreateMaterialRule(const df::StringView& name)->rf::MaterialRuleId {
	MaterialRule* materialRule = m_MaterialRules.Create();
	materialRule->m_ParametersDefinition = DFNew ParamSetDefinition(name, rf::DS_Material);

	return materialRule;
}

void rf::RenderCore::InitMaterialRule(rf::MaterialRuleId materialRule) {
	materialRule->m_ParametersDefinition->CreateRenderData(*this);
}

void rf::RenderCore::DestroyMaterialRule(rf::MaterialRuleId materialRule) {
	materialRule->m_ParametersDefinition->DestroyRenderData(*this);
	DFDelete materialRule->m_ParametersDefinition;
	m_MaterialRules.Destroy(materialRule);
}

auto rf::RenderCore::CreateMaterial(rf::MaterialRuleId materialRule)->rf::MaterialId {
	Material* material = m_Materials.Create();
	material->m_MaterialRule = materialRule;
	material->m_Params = DFNew ParamSet(*materialRule->m_ParametersDefinition);
	material->m_Params->Init(*this, true);

	return material;
}

void rf::RenderCore::DestroyMaterial(rf::MaterialId material) {
	material->m_Params->Release(*this);
	DFDelete material->m_Params;

	m_Materials.Destroy(material);
}

auto rf::RenderCore::RequestPipelineInstance(rf::GraphicsPipelineId pipeline, rf::PassId renderPass)->uint32 {
	for (uint32 i = 0; i < pipeline->m_APIData.m_Instances.size(); ++i) {
		const auto& instance = pipeline->m_APIData.m_Instances[i];
		if (instance.m_RenderPass == renderPass->m_APIData.m_RenderPass) {
			return i;
		}
	}
	return InitPipelineInstance(pipeline, renderPass);
}

auto rf::RenderCore::InitPipelineInstance(rf::GraphicsPipelineId pipeline, rf::PassId renderPass) -> uint32 {
	// Create shader modules
	{
		// VS
		{
			const auto& code = pipeline->m_VSCode;
			auto& handle = pipeline->m_APIData.m_VSModule;

			VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = code.size() * sizeof(uint32);
			shaderModuleCreateInfo.pCode = code.data();

			if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &handle) != VK_SUCCESS) {
				DFAssert(false, "Can't create ShaderModule!");
			}

			DFVkDebugName(m_APIData.m_VkDevice, handle, pipeline->m_Name);
		}

		// FS
		{
			const auto& code = pipeline->m_FSCode;
			auto& handle = pipeline->m_APIData.m_FSModule;

			VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
			shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderModuleCreateInfo.codeSize = code.size() * sizeof(uint32);
			shaderModuleCreateInfo.pCode = code.data();

			if (vk::API::CreateShaderModule(m_APIData.m_VkDevice, &shaderModuleCreateInfo, vk::Allocator(), &handle) != VK_SUCCESS) {
				DFAssert(false, "Can't create ShaderModule!");
			}

			DFVkDebugName(m_APIData.m_VkDevice, handle, pipeline->m_Name);
		}
	}

	// Create Vertex Description
	{
		vk::VertexDescription& description = pipeline->m_APIData.m_VertexDescription;

		for (const auto& attrib : pipeline->m_VertexAttributes) {
			const auto binding = attrib.m_AttributeIndex;
			const auto location = attrib.m_ShaderLocation;
			const auto attribFmt = rf::VertexAttributeRegistry::Entries[attrib.m_AttributeIndex].Format;
			const auto fmt = rf::api::ToVk(attribFmt);

			description.AddBindingDescription(binding, GetStride(attribFmt), VK_VERTEX_INPUT_RATE_VERTEX);
			description.AddAttributeDescription(location, binding, fmt, 0);
		}
	}

	// Create Layout
	{
		const auto& paramSets = pipeline->m_ParameterSets;
		const uint32 descriptorSetLayoutCount = uint32(paramSets.size());

		df::Vector<VkDescriptorSetLayout> descriptorSetLayouts;
		descriptorSetLayouts.resize(descriptorSetLayoutCount);

		for (uint32 i = 0; i < descriptorSetLayoutCount; ++i) {
			const auto dsLayout = paramSets[i]->GetDescriptorSetLayout();
			descriptorSetLayouts[i] = dsLayout->m_Handle;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = descriptorSetLayoutCount;
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		auto& handle = pipeline->m_APIData.m_Layout;
		if (vk::API::CreatePipelineLayout(m_APIData.m_VkDevice, &pipelineLayoutInfo, vk::Allocator(), &handle) != VK_SUCCESS) {
			DFAssert(false, "Can't create PipelineLayout!");
		}

		DFVkDebugName(m_APIData.m_VkDevice, handle, pipeline->m_Name);
	}

	// Create Pipeline
	{
		const char* shaderEntryPoint = "main";

		df::Vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
		if (pipeline->m_APIData.m_VSModule != nullptr) {
			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			pipelineShaderStageCreateInfo.module = pipeline->m_APIData.m_VSModule;
			pipelineShaderStageCreateInfo.pName = shaderEntryPoint;

			shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
		}
		if (pipeline->m_APIData.m_FSModule != nullptr) {
			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
			pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			pipelineShaderStageCreateInfo.module = pipeline->m_APIData.m_FSModule;
			pipelineShaderStageCreateInfo.pName = shaderEntryPoint;

			shaderStageCreateInfos.emplace_back(pipelineShaderStageCreateInfo);
		}

		df::Vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		const uint32 colorAttachmentCount = renderPass->m_APIData.m_ColorAttachmentCount;

		VkPipelineColorBlendStateCreateInfo blendStateCreateInfo;
		df::Vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates;
		ToVkPipelineColorBlendStateCreateInfo(blendStateCreateInfo, blendAttachmentStates, &pipeline->m_BlendState, colorAttachmentCount);

		const auto& vertexDescription = pipeline->m_APIData.m_VertexDescription;
		const auto& vertexInputBindingDescriptions = vertexDescription.GetBindingDescriptions();
		const auto& vertexInputAttributeDescriptions = vertexDescription.GetAttributeDescriptions();

		const VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = ToVkPipelineDepthStencilStateCreateInfo(&pipeline->m_DepthState);
		const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = ToVkPipelineRasterizationStateCreateInfo(&pipeline->m_RasterizationState);

		const uint32 defaultWidth = 4;
		const uint32 defaultHidth = 4;

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexDescription.GetBindingDescriptionCount();
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexDescription.GetAttributeDescriptionCount();
		vertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions;
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
		inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyStateCreateInfo.topology = api::ToVk(EPrimitiveTopology::Triangles);
		inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = float(defaultWidth);
		viewport.height = float(defaultHidth);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent.width = defaultWidth;
		scissor.extent.height = defaultHidth;

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
		multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = uint32(dynamicStates.size());
		dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
		graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCreateInfo.stageCount = uint32(shaderStageCreateInfos.size());
		graphicsPipelineCreateInfo.pStages = shaderStageCreateInfos.data();
		graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
		graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
		graphicsPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
		graphicsPipelineCreateInfo.pColorBlendState = &blendStateCreateInfo;
		graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
		graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		graphicsPipelineCreateInfo.layout = pipeline->m_APIData.m_Layout;
		graphicsPipelineCreateInfo.renderPass = renderPass->m_APIData.m_RenderPass;
		graphicsPipelineCreateInfo.subpass = 0;
		graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

		rf::api::GraphicsPipeline::PipelineInstance instance = {};
		instance.m_RenderPass = renderPass->m_APIData.m_RenderPass;

		auto& handle = instance.m_Pipeline;
		if (vk::API::CreateGraphicsPipelines(m_APIData.m_VkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, vk::Allocator(), &handle) != VK_SUCCESS) {
			DFAssert(false, "Can't create GraphicsPipeline!");
		}

		DFVkDebugName(m_APIData.m_VkDevice, handle, pipeline->m_Name);

		pipeline->m_APIData.m_Instances.emplace_back(instance);
	}

	return static_cast<uint32>(pipeline->m_APIData.m_Instances.size() - 1);
}









bool rf::RenderCore::InitInstance() {
	DFAssert(m_APIData.m_VkInstance == VK_NULL_HANDLE, "Instance already created!");

	VkApplicationInfo applicationCreateInfo = {};
	applicationCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationCreateInfo.pApplicationName = "Game";
	applicationCreateInfo.pEngineName = "Engine";
	applicationCreateInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationCreateInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationCreateInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &applicationCreateInfo;
	instanceCreateInfo.ppEnabledExtensionNames = vk::GetRequiredInstanceExtensions();
	instanceCreateInfo.enabledExtensionCount = vk::GetNumRequiredInstanceExtensions();
	instanceCreateInfo.ppEnabledLayerNames = vk::GetRequiredValidationLayers();
	instanceCreateInfo.enabledLayerCount = vk::GetNumRequiredValidationLayers();

	const VkResult result = vk::API::CreateInstance(&instanceCreateInfo, vk::Allocator(), &m_APIData.m_VkInstance);
	if (result != VK_SUCCESS) {
		DFAssert(false, vk::GetErrorString(result));

		return false;
	}

	vk::API::LoadInstanceFunctions(m_APIData.m_VkInstance);

	if (!vk::CreateDebugCallback(m_APIData.m_VkInstance)) {
		return false;
	}

	return true;
}

bool rf::RenderCore::InitPhysicalDevice() {
	uint32 physicalDeviceCount = 0;
	vk::API::EnumeratePhysicalDevices(m_APIData.m_VkInstance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0) {
		DFAssert(false, "Failed to find any physical device!");

		return false;
	}

	df::Vector<VkPhysicalDevice> devices = df::Vector<VkPhysicalDevice>(physicalDeviceCount);
	vk::API::EnumeratePhysicalDevices(m_APIData.m_VkInstance, &physicalDeviceCount, devices.data());

	for (uint32 i = 0; i < physicalDeviceCount; ++i) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vk::API::GetPhysicalDeviceProperties(devices[i], &deviceProperties);
		vk::API::GetPhysicalDeviceFeatures(devices[i], &deviceFeatures);

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			m_APIData.m_VkPhysicalDevice = devices[i];

			m_APIData.m_DeviceProperties = deviceProperties;
			m_APIData.m_DeviceFeatures = deviceFeatures;
			m_APIData.m_DeviceLimits = deviceProperties.limits;
		}
	}

	if (m_APIData.m_VkPhysicalDevice == VK_NULL_HANDLE) {
		DFAssert(false, "There is no compatible PhysicalDevice!");

		return false;
	}


	uint32 queueFamilyCount = 0;
	vk::API::GetPhysicalDeviceQueueFamilyProperties(m_APIData.m_VkPhysicalDevice, &queueFamilyCount, nullptr);

	if (queueFamilyCount == 0) {
		DFAssert(false, "Failed to find any QueueFamily!");

		return false;
	}

	df::Vector<VkQueueFamilyProperties> queueFamilies = df::Vector<VkQueueFamilyProperties>(queueFamilyCount);
	vk::API::GetPhysicalDeviceQueueFamilyProperties(m_APIData.m_VkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	for (uint32 i = 0; i < queueFamilyCount; ++i) {
		VkQueueFamilyProperties queueFamily = queueFamilies[i];

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			m_APIData.m_GraphicsFamilyIndex = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			m_APIData.m_TransferFamilyIndex = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			m_APIData.m_ComputeFamilyIndex = i;
		}

		const VkBool32 presentSupport = m_APIData.m_Presentation.GetPhysicalDeviceSurfaceSupport(m_APIData.m_VkPhysicalDevice, i);

		if (queueFamily.queueCount > 0 && presentSupport) {
			m_APIData.m_PresentFamilyIndex = i;
		}

		if (m_APIData.m_PresentFamilyIndex != uint32(-1) &&
			m_APIData.m_GraphicsFamilyIndex != uint32(-1) &&
			m_APIData.m_TransferFamilyIndex != uint32(-1) &&
			m_APIData.m_ComputeFamilyIndex != uint32(-1)) {
			vk::SetupMemoryProperties(m_APIData.m_VkPhysicalDevice);
			break;
		}
	}

	return true;
}

bool rf::RenderCore::InitDevice() {
	DFAssert(m_APIData.m_VkDevice == VK_NULL_HANDLE, "Device already created!");

	uint32 uniqueQueues[vk::Constants::MaxQueueFamilyIndices];

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		uniqueQueues[i] = uint32(-1);
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_APIData.m_PresentFamilyIndex) {
			uniqueQueues[i] = m_APIData.m_PresentFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_APIData.m_GraphicsFamilyIndex) {
			uniqueQueues[i] = m_APIData.m_GraphicsFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_APIData.m_TransferFamilyIndex) {
			uniqueQueues[i] = m_APIData.m_TransferFamilyIndex;
			break;
		}
	}

	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i) {
		if (uniqueQueues[i] == uint32(-1) || uniqueQueues[i] == m_APIData.m_ComputeFamilyIndex) {
			uniqueQueues[i] = m_APIData.m_ComputeFamilyIndex;
			break;
		}
	}

	float queuePriority = 1.0f;
	uint32 queueFamilyCount = 0;
	VkDeviceQueueCreateInfo queueCreateInfos[vk::Constants::MaxQueueFamilyIndices];
	for (uint32 i = 0; i < vk::Constants::MaxQueueFamilyIndices; ++i, ++queueFamilyCount) {
		if (uniqueQueues[i] == uint32(-1)) {
			break;
		}

		queueCreateInfos[i] = {};
		queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfos[i].queueFamilyIndex = uniqueQueues[i];
		queueCreateInfos[i].queueCount = 1;
		queueCreateInfos[i].pQueuePriorities = &queuePriority;
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	vk::API::GetPhysicalDeviceFeatures(m_APIData.m_VkPhysicalDevice, &deviceFeatures);

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = queueFamilyCount;
	createInfo.pQueueCreateInfos = queueCreateInfos;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = vk::GetNumRequiredDeviceExtensions();
	createInfo.ppEnabledExtensionNames = vk::GetRequiredDeviceExtensions();
	createInfo.enabledLayerCount = vk::GetNumRequiredValidationLayers();
	createInfo.ppEnabledLayerNames = vk::GetRequiredValidationLayers();

	if (vk::API::CreateDevice(m_APIData.m_VkPhysicalDevice, &createInfo, vk::Allocator(), &m_APIData.m_VkDevice) != VK_SUCCESS) {
		DFAssert(false, "Can't create Device!");

		return false;
	}

	if (!vk::API::LoadDeviceFunctions(m_APIData.m_VkDevice)) {
		return false;
	}

	return true;
}


bool rf::RenderCore::CheckForPreventedCaptureSoft() {
#define DF_PREVENT_RENDERDOC 0
#if DF_PREVENT_RENDERDOC
	df::Vector<df::String> preventedLayers = {
		"VK_LAYER_RENDERDOC_Capture"
	};

	uint32 layerPropertyCount = 0;
	df::Vector<VkLayerProperties> layerProperties;
	vk::API::EnumerateDeviceLayerProperties(m_APIData.m_VkPhysicalDevice, &layerPropertyCount, nullptr);
	layerProperties.resize(layerPropertyCount);
	vk::API::EnumerateDeviceLayerProperties(m_APIData.m_VkPhysicalDevice, &layerPropertyCount, layerProperties.data());

	for (const auto& layer : layerProperties) {
		for (const auto& preventedLayer : preventedLayers) {
			if (preventedLayer == layer.layerName) {
				return false;
			}
		}
	}
#endif

	return true;
}


bool rf::RenderCore::InitCommandPools() {
	DFAssert(m_APIData.m_GraphicsCommandPools.empty(), "GraphicsCommandPools already created!");
	DFAssert(m_APIData.m_TransferCommandPools.empty(), "TransferCommandPools already created!");

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	// Graphics
	{
		poolInfo.queueFamilyIndex = m_APIData.m_GraphicsFamilyIndex;
		m_APIData.m_GraphicsCommandPools.resize(1);
		for (VkCommandPool& commandPool : m_APIData.m_GraphicsCommandPools) {
			if (vk::API::CreateCommandPool(m_APIData.m_VkDevice, &poolInfo, vk::Allocator(), &commandPool) != VK_SUCCESS) {
				DFAssert(false, "Can't create GraphicsCommandPool!");

				return false;
			}

			DFVkDebugName(m_APIData.m_VkDevice, commandPool, "GraphicsCommandPool");
		}
	}

	// Transfer
	{
		poolInfo.queueFamilyIndex = m_APIData.m_TransferFamilyIndex;
		m_APIData.m_TransferCommandPools.resize(1);
		for (VkCommandPool& commandPool : m_APIData.m_TransferCommandPools) {
			if (vk::API::CreateCommandPool(m_APIData.m_VkDevice, &poolInfo, vk::Allocator(), &commandPool) != VK_SUCCESS) {
				DFAssert(false, "Can't create TransferCommandPool!");

				return false;
			}

			DFVkDebugName(m_APIData.m_VkDevice, commandPool, "TransferCommandPool");
		}
	}

	return true;
}

void rf::RenderCore::ReleaseCommandPools() {
	for (VkCommandPool commandPool : m_APIData.m_GraphicsCommandPools) {
		m_APIData.m_VulkanObjectManager.RemoveCommandPool(m_APIData.m_VkDevice, commandPool);
	}
	m_APIData.m_GraphicsCommandPools.clear();

	for (VkCommandPool commandPool : m_APIData.m_TransferCommandPools) {
		m_APIData.m_VulkanObjectManager.RemoveCommandPool(m_APIData.m_VkDevice, commandPool);
	}
	m_APIData.m_TransferCommandPools.clear();
}

bool rf::RenderCore::InitDescriptorPools() {

	const uint32 maxDescriptorSets = 800;
	df::Vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.push_back({ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , 3 });

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
	descriptorPoolCreateInfo.maxSets = maxDescriptorSets;


	if (vk::API::CreateDescriptorPool(m_APIData.m_VkDevice, &descriptorPoolCreateInfo, vk::Allocator(), &m_APIData.m_DescriptorPool) != VK_SUCCESS) {
		DFAssert(false, "Can't create DescriptorPool!");
		return false;
	}

	return true;
}

void rf::RenderCore::ReleaseDescriptorPools() {
	m_APIData.m_VulkanObjectManager.RemoveDescriptorPool(m_APIData.m_VkDevice, m_APIData.m_DescriptorPool);
}


bool rf::RenderCore::InitFrameData(rf::FrameData& frameData) {
	if (!InitCommandBuffers(frameData)) {
		return false;
	}

	frameData.m_APIData.m_ImageAvailableSemaphore = vk::CreateSemaphore(m_APIData.m_VkDevice, 0);
	frameData.m_APIData.m_RenderFinishedSemaphore = vk::CreateSemaphore(m_APIData.m_VkDevice, 0);

	frameData.m_APIData.m_InFlightFence = vk::CreateFence(m_APIData.m_VkDevice, VK_FENCE_CREATE_SIGNALED_BIT);

	return true;
}

void rf::RenderCore::ReleaseFrameData(rf::FrameData& frameData) {
	vk::DestroyFence(m_APIData.m_VkDevice, frameData.m_APIData.m_InFlightFence);

	vk::DestroySemaphore(m_APIData.m_VkDevice, frameData.m_APIData.m_RenderFinishedSemaphore);
	vk::DestroySemaphore(m_APIData.m_VkDevice, frameData.m_APIData.m_ImageAvailableSemaphore);

	ReleaseCommandBuffers(frameData);
}

bool rf::RenderCore::InitCommandBuffers(rf::FrameData& frameData) {
	DFAssert(!m_APIData.m_GraphicsCommandPools.empty(), "GraphicsCommandPools not created!");
	DFAssert(!m_APIData.m_TransferCommandPools.empty(), "TransferCommandPools not created!");

	// Graphics
	frameData.m_CommandBuffer.GetAPIData().Init("GraphicsCommandBuffer", m_APIData.m_VkDevice, m_APIData.m_GraphicsCommandPools[0]);

	return true;
}

void rf::RenderCore::ReleaseCommandBuffers(rf::FrameData& frameData) {
	frameData.m_CommandBuffer.GetAPIData().Release(m_APIData.m_VkDevice, m_APIData.m_GraphicsCommandPools[0]);
}









































rf::api::RenderCore::RenderCore() 
	: m_VkInstance(VK_NULL_HANDLE)
	, m_VkPhysicalDevice(VK_NULL_HANDLE)
	, m_VkDevice(VK_NULL_HANDLE) 
	, m_GraphicsQueue(VK_NULL_HANDLE)
	, m_PresentQueue(VK_NULL_HANDLE)
	, m_TransferQueue(VK_NULL_HANDLE)
	, m_GraphicsFamilyIndex(0)
	, m_PresentFamilyIndex(0)
	, m_TransferFamilyIndex(0)
{
}

rf::api::RenderCore::~RenderCore() {
	
}

void rf::api::RenderCore::InitSampler(rf::SamplerId sampler, const df::StringView& name, const rf::SamplerState& samplerState) {
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = api::ToVk(samplerState.m_Filter);
	samplerCreateInfo.minFilter = api::ToVk(samplerState.m_Filter);
	samplerCreateInfo.addressModeU = api::ToVk(samplerState.m_AddressMode);
	samplerCreateInfo.addressModeV = api::ToVk(samplerState.m_AddressMode);
	samplerCreateInfo.addressModeW = api::ToVk(samplerState.m_AddressMode);
	samplerCreateInfo.anisotropyEnable = VK_TRUE;
	samplerCreateInfo.maxAnisotropy = 16;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.mipmapMode = api::ToVk(samplerState.m_MipmapMode);

	if (vk::API::CreateSampler(m_VkDevice, &samplerCreateInfo, vk::Allocator(), &sampler->m_Handle) != VK_SUCCESS) {
		DFAssert(false, "Can't create Sampler!");
	}

	DFVkDebugName(m_VkDevice, sampler->m_Handle, name);
}

void rf::api::RenderCore::ReleaseSampler(rf::SamplerId sampler) {
	m_VulkanObjectManager.RemoveSampler(m_VkDevice, sampler->m_Handle);
}

void rf::api::RenderCore::InitRenderPass(
	rf::PassId renderPass,
	const df::StringView& name,
	const df::Vector<VkAttachmentDescription>& attachementDescriptions,
	const df::Vector<rf::ClearValue>& clearValues) {

	df::Vector<VkAttachmentReference> attachmentReferences;

	attachmentReferences.resize(attachementDescriptions.size());
	renderPass->m_APIData.m_ClearValues.resize(attachementDescriptions.size());

	VkAttachmentReference* depthStensilAttachmentRef = nullptr;
	const uint32 attachmentCount = uint32(attachementDescriptions.size());

	for (uint32 i = 0; i < attachmentCount; ++i) {
		const VkAttachmentDescription& desc = attachementDescriptions[i];

		const bool isDepthStencil = vk::IsDepth(desc.format) || vk::IsStencil(desc.format);

		VkAttachmentReference& ref = isDepthStencil
			? attachmentReferences[attachmentCount - 1]
			: attachmentReferences[i];

		ref = {};
		ref.attachment = i;
		ref.layout = isDepthStencil
			? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		if (isDepthStencil) {
			DFAssert(depthStensilAttachmentRef == false, "Multiple depth stencil definitions!");
			depthStensilAttachmentRef = &ref;
		}

		static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i]) == sizeof(clearValues[i]), "ClearVlaue incompatible!");
		static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i].color) == sizeof(clearValues[i].ColorValue), "ClearVlaue incompatible!");
		static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i].depthStencil) == sizeof(clearValues[i].DepthStencilValue), "ClearVlaue incompatible!");
		df::MemCpy(&renderPass->m_APIData.m_ClearValues[i], &clearValues[i], sizeof(clearValues[i]));
	}

	renderPass->m_APIData.m_ColorAttachmentCount = (depthStensilAttachmentRef == nullptr)
		? uint32(attachmentReferences.size())
		: uint32(attachmentReferences.size() - 1);

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = (depthStensilAttachmentRef == nullptr)
		? uint32(attachmentReferences.size())
		: uint32(attachmentReferences.size() - 1);
	subpassDescription.pColorAttachments = attachmentReferences.data();
	subpassDescription.pDepthStencilAttachment = depthStensilAttachmentRef;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = uint32(attachementDescriptions.size());
	renderPassCreateInfo.pAttachments = attachementDescriptions.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	if (vk::API::CreateRenderPass(m_VkDevice, &renderPassCreateInfo, vk::Allocator(), &renderPass->m_APIData.m_RenderPass) != VK_SUCCESS) {
		DFAssert(false, "Can't create RenderPass!");
	}

	DFVkDebugName(m_VkDevice, renderPass->m_APIData.m_RenderPass, name);
}

void rf::api::RenderCore::ReleaseRenderPass(rf::PassId renderPass) {
	renderPass->m_APIData.m_ClearValues.clear();

	m_VulkanObjectManager.RemoveRenderPass(m_VkDevice, renderPass->m_APIData.m_RenderPass);
}

void rf::api::RenderCore::InitFramebuffer(rf::FramebufferId framebuffer, const df::StringView& name, rf::PassId renderPass, const df::Vector<VkImageView>& attachments, uint32 width, uint32 height) {
	framebuffer->m_APIData.m_Extends.width = width;
	framebuffer->m_APIData.m_Extends.height = height;

	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.renderPass = renderPass->m_APIData.m_RenderPass;
	framebufferCreateInfo.width = width;
	framebufferCreateInfo.height = height;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.pAttachments = attachments.data();
	framebufferCreateInfo.attachmentCount = uint32(attachments.size());

	if (vk::API::CreateFramebuffer(m_VkDevice, &framebufferCreateInfo, vk::Allocator(), &framebuffer->m_APIData.m_Framebuffer) != VK_SUCCESS) {
		DFAssert(false, "Can't create RenderPass!");
	}

	DFVkDebugName(m_VkDevice, framebuffer->m_APIData.m_Framebuffer, name);
}

void rf::api::RenderCore::ReleaseFramebuffer(rf::FramebufferId framebuffer) {
	m_VulkanObjectManager.RemoveFramebuffer(m_VkDevice, framebuffer->m_APIData.m_Framebuffer);
}

void rf::api::RenderCore::EmptyTrashCan() {
	while (m_VulkanObjectManager.Update()) {}
}