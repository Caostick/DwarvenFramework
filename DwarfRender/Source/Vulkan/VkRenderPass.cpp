#include "VkRenderPass.h"
#include "VkDebug.h"
#include "VkAllocator.h"
#include "VkRenderCore.h"

#include <DwarvenCore/Assert.h>

vk::RenderPass::RenderPass(vk::RenderCore& renderCore) 
	: m_RenderCore(renderCore)
	, m_IsBuilt(false)
	, m_VkRenderPass(VK_NULL_HANDLE)
	, m_VkFramebuffer(VK_NULL_HANDLE)
	, m_VkExtents({ 0, 0 })
	, m_ColorAttachmnets()
	, m_DepthStencilAttachment() {
}

vk::RenderPass::~RenderPass() {
	if (m_VkRenderPass != VK_NULL_HANDLE) {
		m_RenderCore.RemoveRenderPass(m_VkRenderPass);
	}

	if (m_VkFramebuffer != VK_NULL_HANDLE) {
		m_RenderCore.RemoveFramebuffer(m_VkFramebuffer);
	}
}

void vk::RenderPass::SetName(const df::StringView& name) {
	m_Name = name;
}

void vk::RenderPass::Validate() {
	if (m_IsBuilt) {
		return;
	}

	Build();
}

void vk::RenderPass::SetExtents(uint32 width, uint32 height) {
	m_VkExtents.width = width;
	m_VkExtents.height = height;
}

void vk::RenderPass::SetColorTarget(uint32 index, VkImageView imageView, const VkAttachmentDescription& description, const VkClearValue& clearValue) {
	if (m_ColorAttachmnets.size() <= index) {
		m_ColorAttachmnets.resize(index + 1);
	}

	m_ColorAttachmnets[index].ImageView = imageView;
	m_ColorAttachmnets[index].Description = description;
	m_ColorAttachmnets[index].ClearValue = clearValue;

	//static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i]) == sizeof(clearValues[i]), "ClearVlaue incompatible!");
	//static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i].color) == sizeof(clearValues[i].ColorValue), "ClearVlaue incompatible!");
	//static_assert(sizeof(renderPass->m_APIData.m_ClearValues[i].depthStencil) == sizeof(clearValues[i].DepthStencilValue), "ClearVlaue incompatible!");

	m_IsBuilt = false;
}

void vk::RenderPass::SetDepthStencilTarget(VkImageView imageView, const VkAttachmentDescription& description, const VkClearValue& clearValue) {
	m_DepthStencilAttachment.ImageView = imageView;
	m_DepthStencilAttachment.Description = description;
	m_DepthStencilAttachment.ClearValue = clearValue;

	m_IsBuilt = false;
}

bool vk::RenderPass::HasDepthStencil() const {
	return m_DepthStencilAttachment.ImageView != VK_NULL_HANDLE;
}

auto vk::RenderPass::GetVkRenderPass() const->VkRenderPass {
	DFAssert(m_IsBuilt, "Render pass is not built!");

	return m_VkRenderPass;
}

auto vk::RenderPass::GetVkFramebuffer() const->VkFramebuffer {
	DFAssert(m_IsBuilt, "Render pass is not built!");

	return m_VkFramebuffer;
}

auto vk::RenderPass::GetVkClearValues() const -> const df::Vector<VkClearValue>& {
	return m_ClearValues;
}

auto vk::RenderPass::GetVkExtents() const->const VkExtent2D& {
	return m_VkExtents;
}

void vk::RenderPass::Build() {
	BuildRenderPass();
	BuildFramebuffer();

	m_IsBuilt = true;
}

void vk::RenderPass::BuildRenderPass() {
	if (m_VkRenderPass != VK_NULL_HANDLE) {
		m_RenderCore.RemoveRenderPass(m_VkRenderPass);
		m_VkRenderPass = VK_NULL_HANDLE;
	}

	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	const bool hasDepth = HasDepthStencil();
	const size_t colorAttachmentCount = m_ColorAttachmnets.size();
	const size_t attachmentCount = colorAttachmentCount + (hasDepth ? 1 : 0);

	df::Vector<VkAttachmentDescription> attachmentDescriptions;
	df::Vector<VkAttachmentReference> attachmentReferences;

	VkAttachmentReference* depthStencilAttachmentReference = nullptr;

	attachmentDescriptions.resize(attachmentCount);
	attachmentReferences.resize(attachmentCount);
	m_ClearValues.resize(attachmentCount);

	for (uint32 i = 0; i < m_ColorAttachmnets.size(); ++i) {
		VkAttachmentReference& ref = attachmentReferences[i];
		ref = {};
		ref.attachment = i;
		ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		m_ClearValues[i] = m_ColorAttachmnets[i].ClearValue;
		attachmentDescriptions[i] = m_ColorAttachmnets[i].Description;
	}

	if(hasDepth) {
		VkAttachmentReference& ref = attachmentReferences[attachmentCount - 1];
		ref = {};
		ref.attachment = uint32(attachmentCount - 1);
		ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		m_ClearValues[attachmentCount - 1] = m_DepthStencilAttachment.ClearValue;
		attachmentDescriptions[attachmentCount - 1] = m_DepthStencilAttachment.Description;
		depthStencilAttachmentReference = &ref;
	}

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = uint32(colorAttachmentCount);
	subpassDescription.pColorAttachments = attachmentReferences.data();
	subpassDescription.pDepthStencilAttachment = depthStencilAttachmentReference;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = uint32(attachmentDescriptions.size());
	renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	if (vk::API::CreateRenderPass(vkDevice, &renderPassCreateInfo, vk::Allocator(), &m_VkRenderPass) != VK_SUCCESS) {
		DFAssert(false, "Can't create RenderPass!");
	}

	DFVkDebugName(vkDevice, m_VkRenderPass, m_Name);
}

void vk::RenderPass::BuildFramebuffer() {
	if (m_VkFramebuffer != VK_NULL_HANDLE) {
		m_RenderCore.RemoveFramebuffer(m_VkFramebuffer);
		m_VkFramebuffer = VK_NULL_HANDLE;
	}

	DFAssert(m_VkRenderPass != VK_NULL_HANDLE, "VkRenderPass wasn't initialized!");
	DFAssert(m_VkExtents.width != 0 && m_VkExtents.height != 0, "Invalid extends!");

	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	const bool hasDepth = HasDepthStencil();
	const size_t colorAttachmentCount = m_ColorAttachmnets.size();
	const size_t attachmentCount = colorAttachmentCount + (hasDepth ? 1 : 0);

	df::Vector<VkImageView> attachments;

	attachments.resize(attachmentCount);

	for (uint32 i = 0; i < m_ColorAttachmnets.size(); ++i) {
		attachments[i] = m_ColorAttachmnets[i].ImageView;
	}

	if (hasDepth) {
		attachments[attachmentCount - 1] = m_DepthStencilAttachment.ImageView;
	}

	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.renderPass = m_VkRenderPass;
	framebufferCreateInfo.width = m_VkExtents.width;
	framebufferCreateInfo.height = m_VkExtents.height;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.pAttachments = attachments.data();
	framebufferCreateInfo.attachmentCount = uint32(attachments.size());

	if (vk::API::CreateFramebuffer(vkDevice, &framebufferCreateInfo, vk::Allocator(), &m_VkFramebuffer) != VK_SUCCESS) {
		DFAssert(false, "Can't create Framebuffer!");
	}

	DFVkDebugName(vkDevice, m_VkFramebuffer, m_Name);
}