#pragma once

#include <DwarfRender/RenderPass.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#include "VkAPI.h"

namespace vk {
	class RenderCore;
}

namespace vk {
	enum class EAttachmentLoadOp {
		DontCare,
		Clear,
		Load,

		COUNT
	};

	enum class EAttachmentStoreOp {
		DontCare,
		Store,

		COUNT
	};

	union ClearColorValue {
		float       Float32[4];
		int32_t     Int32[4];
		uint32_t    UInt32[4];
	};

	struct ClearDepthStencilValue {
		float       Depth;
		uint32_t    Stencil;
	};

	union ClearValue {
		ClearColorValue ColorValue;
		ClearDepthStencilValue DepthStencilValue;
	};

	struct AttachmentInfo {
		VkImageView ImageView = VK_NULL_HANDLE;
		VkAttachmentDescription Description = {};
		VkClearValue ClearValue = {};
	};
}

namespace vk {
	class RenderPass : public df::RenderPass {
	public:
		RenderPass(vk::RenderCore& renderCore);
		~RenderPass() override;

		virtual void SetName(const df::StringView& name) override;

		virtual void SetTarget(uint32 index, df::Texture* texture, df::ERenderTargetOp operation, const df::ClearColor& clearValue) override;
		virtual void SetDepthStencilTarget(df::Texture* texture, df::ERenderTargetOp operation, const df::ClearDepth& clearValue) override;

		void Validate();

		void SetExtents(uint32 width, uint32 height);

		void SetColorTarget(uint32 index, VkImageView imageView, const VkAttachmentDescription& description, const VkClearValue& clearValue);
		void SetDepthStencilTarget(VkImageView imageView, const VkAttachmentDescription& description, const VkClearValue& clearValue);

		bool HasDepthStencil() const;

		auto GetVkRenderPass() const->VkRenderPass;
		auto GetVkFramebuffer() const->VkFramebuffer;
		auto GetVkClearValues() const -> const df::Vector<VkClearValue>&;
		auto GetVkExtents() const->const VkExtent2D&;

		auto GetColorAttachmentCount() const -> uint32;

	private:
		void Build();
		void BuildRenderPass();
		void BuildFramebuffer();

		vk::RenderCore& m_RenderCore;
		df::String m_Name;

		bool m_FramebufferIsBuilt;
		bool m_RenderPassIsBuilt;

		VkRenderPass m_VkRenderPass;
		VkFramebuffer m_VkFramebuffer;
		VkExtent2D m_VkExtents;

		df::Vector<AttachmentInfo> m_ColorAttachmnets;
		AttachmentInfo m_DepthStencilAttachment;

		df::Vector<VkClearValue> m_ClearValues;
	};
}