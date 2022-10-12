#include <DwarfRender/RenderView.h>
#include <DwarfRender/GlobalObjects.h>
#include <DwarfRender/RenderPass.h>
#include <DwarfRender/ParamSet.h>
#include <DwarfRender/ModelStream.h>

#include "RenderPassInstance.h"
#include "RenderCore.h"
#include "Texture.h"
#include "CommandBuffer.h"
#include "DescriptorSetDefinitions.h"

#include "Vulkan/VkRenderContext.h"

#include <DwarvenCore/New.h>
#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Log.h>

rf::RenderView::~RenderView() {}

void rf::RenderView::Init(rf::RenderCore& renderCore) {
	m_RenderTargets.resize(rf::RenderTargetRegistry::TypeCount);
	for (auto&& rt : m_RenderTargets) {
		rt = nullptr;
	}

	for (auto renderPass : m_RenderPasses) {
		const auto rpId = renderPass->m_TypeId;
		const auto& entry = rf::RenderPassRegistry::Entries[rpId];

		df::Vector<rf::TextureId> textures;
		textures.resize(entry.m_OutputRenderTargetCount);
		for (uint32 i = 0; i < entry.m_OutputRenderTargetCount; ++i) {
			const auto& rt = entry.m_OutputRenderTargets[i];
			textures[i] = RequestRenderTarget(renderCore, rt);
		}

		renderPass->Init(renderCore, textures);
	}
}

void rf::RenderView::Release(rf::RenderCore& renderCore) {
	for (auto renderPass : m_RenderPasses) {
		renderCore.DestroyFramebuffer(renderPass->m_Framebuffer);
		renderPass->Release(renderCore);
		DFDelete renderPass;
	}

	for (auto tex : m_RenderTargets) {
		if (tex) {
			renderCore.DestroyTexture(tex);
		}
	}
	m_RenderTargets.clear();

	ReleaseParams(renderCore);
}

auto rf::RenderView::CreateModelStream()->rf::ModelStream* {
	rf::ModelStream* modelStream = DFNew rf::ModelStream;
	m_ModelStreams.push_back(modelStream);

	return modelStream;
}

void rf::RenderView::DestroyModelStream(rf::ModelStream* modelStream) {
	// @TODO: remove references from RenderPasses

	df::RemoveOrdered(m_ModelStreams, modelStream);
	DFDelete modelStream;
}

bool rf::RenderView::RenderFrame(const rf::RenderContext& renderContext) {
	for (rf::TextureId texture : m_RenderTargetsToInit) {
		const bool isDepth = (texture->m_Format == rf::ETextureFormat::D32_Float || texture->m_Format == rf::ETextureFormat::D24_Float_S8_UInt);
		if (isDepth) {
			renderContext.m_CommandBuffer->TransitionImageLayout(texture, EImageLayout::Undefined, EImageLayout::DepthStencilReadOnly);
		} else {
			renderContext.m_CommandBuffer->TransitionImageLayout(texture, EImageLayout::Undefined, EImageLayout::ColorReadOnly);
		}
	}
	m_RenderTargetsToInit.clear();

	UpdateParams(renderContext.m_FrameIndex);

	for (auto renderPass : m_RenderPasses) {
		renderPass->UpdateParams();
	}

	for (auto renderPass : m_RenderPasses) {
		const auto& entry = rf::RenderPassRegistry::Entries[renderPass->m_TypeId];

		DFScopedRenderEvent((*renderContext.m_CommandBuffer), entry.m_Name);

		renderContext.m_CommandBuffer->BeginRenderPass(entry.m_RenderPass, renderPass->m_Framebuffer);
		renderContext.m_CommandBuffer->EndRenderPass();
	}

	// After rendering
	for (auto modelStream : m_ModelStreams) {
		modelStream->Reset();
	}

	return true;
}

auto rf::RenderView::GetOutput() const->rf::TextureId {
	return rf::GlobalObjects::Get(ETexture::Null);
}

auto rf::RenderView::RequestRenderTarget(rf::RenderCore& renderCore, uint32 renderTargetId)->rf::TextureId {
	if (!m_RenderTargets[renderTargetId]) {
		const auto& entry = rf::RenderTargetRegistry::Entries[renderTargetId];

		const bool isDepthStencil =
			(entry.m_Format == ETextureFormat::D32_Float) ||
			(entry.m_Format == ETextureFormat::D24_Float_S8_UInt);

		rf::ImageUsageFlags flags = isDepthStencil
			? rf::EImageUsageFlag::DepthRenderTarget
			: rf::EImageUsageFlag::RenderTarget;

		rf::TextureId tex = renderCore.CreateTexture(entry.m_Name, 128, 128, flags, entry.m_Format);
		m_RenderTargets[renderTargetId] = tex;

		m_RenderTargetsToInit.push_back(tex);
	}

	return m_RenderTargets[renderTargetId];
}

void rf::RenderView::AddRenderPass(rf::RenderPass* renderPass, uint32 typeId) {
	RenderPassInstance* container = DFNew RenderPassInstance(renderPass, typeId);
	m_RenderPasses.push_back(container);
}