#pragma once

#include "VkAPI.h"
#include "VkVertexDescription.h"
#include "VkPresentation.h"
#include "VkBuffer.h"
#include "VkTexture.h"
#include "VkPass.h"
#include "VkFramebuffer.h"
#include "VkObjectManager.h"
#include "VkTransferBuffer.h"
#include "VkRenderContext.h"

#include "../FrameData.h"
#include "../Texture.h"
#include "../Mesh.h"
#include "../ShaderModule.h"
#include "../VertexDescription.h"
#include "../Pass.h"
#include "../Framebuffer.h"
#include "../Buffer.h"
#include "../Sampler.h"
#include "../Descriptor.h"
#include "../DescriptorSet.h"
#include "../DescriptorSetLayout.h"
#include "../Pipeline.h"
#include "../GraphicsPipeline.h"
#include "../PipelineLayout.h"

#include <DwarfConstructorRender/ObjectId.h>
#include <DwarfConstructorRender/SamplerState.h>
#include <DwarfConstructorRender/RasterizationState.h>
#include <DwarfConstructorRender/PrimitiveTopology.h>
#include <DwarfConstructorRender/BlendState.h>
#include <DwarfConstructorRender/DepthState.h>
#include <DwarfConstructorRender/PipelineDescriptor.h>
#include <DwarfConstructorRender/MaterialRule.h>
#include <DwarfConstructorRender/Material.h>

#include <DwarvenCore/SparseArray.h>
#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

#include <DwarvenCore/Math/Math.h>

namespace df {
	class Window;
}

namespace rf {
	namespace api {
		struct RenderCore {
		public:
			RenderCore();
			~RenderCore();

			void InitSampler(rf::SamplerId sampler, const df::StringView& name, const rf::SamplerState& samplerState);
			void ReleaseSampler(rf::SamplerId sampler);

			void InitRenderPass(rf::PassId renderPass, const df::StringView& name, const df::Vector<VkAttachmentDescription>& attachementDescriptions, const df::Vector<rf::ClearValue>& clearValues);
			void ReleaseRenderPass(rf::PassId renderPass);

			void InitFramebuffer(rf::FramebufferId framebuffer, const df::StringView& name, rf::PassId renderPass, const df::Vector<VkImageView>& attachments, uint32 width, uint32 height);
			void ReleaseFramebuffer(rf::FramebufferId framebuffer);

			void EmptyTrashCan();

		public:
			VkInstance m_VkInstance;
			VkPhysicalDevice m_VkPhysicalDevice;
			VkDevice m_VkDevice;

			uint32 m_PresentFamilyIndex;
			uint32 m_GraphicsFamilyIndex;
			uint32 m_TransferFamilyIndex;
			uint32 m_ComputeFamilyIndex;

			VkQueue m_PresentQueue;
			VkQueue m_GraphicsQueue;
			VkQueue m_TransferQueue;
			VkQueue m_ComputeQueue;

			df::Vector<VkCommandPool> m_GraphicsCommandPools;
			df::Vector<VkCommandPool> m_TransferCommandPools;

			rf::api::Presentation m_Presentation;
			rf::api::TransferBuffer m_TransferBuffer;

			VkDescriptorPool m_DescriptorPool;

			VkPhysicalDeviceProperties m_DeviceProperties;
			VkPhysicalDeviceFeatures m_DeviceFeatures;
			VkPhysicalDeviceLimits m_DeviceLimits;

			vk::ObjectManager m_VulkanObjectManager;
		};
	}
}