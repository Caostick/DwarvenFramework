#include <DwarfRender/Renderer.h>
#include <DwarfRender/GlobalObjects.h>
#include <DwarfRender/ParamSet.h>
#include <DwarfRender/ParamSetDefinition.h>
#include <DwarfRender/ParamSetDeclarator.h>
#include <DwarfRender/RenderView.h>
#include <DwarfRender/RenderPass.h>

#include <DwarfWindow/Window.h>

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

#include "RenderCore.h"
#include "DescriptorSetDefinitions.h"

rf::Renderer::Renderer() 
	: m_RenderCore(nullptr)
{}

rf::Renderer::~Renderer() {}

bool rf::Renderer::Init(df::Window* window) {

	m_Window = window;

	m_FramebufferWidth = m_Window->GetWidth();
	m_FramebufferHeight = m_Window->GetHeight();

	if (!InitRenderCore()) {
		DFAssert(false, "Can't create render core!");
		return false;
	}

	InitGlobalShaderData();

	CheckRenderPassFormats();

	return true;
}

void rf::Renderer::Release() {
	for (auto&& rv : m_RenderViews) {
		rv->Release(*m_RenderCore);
		DFDelete rv;
	}
	m_RenderViews.clear();
	m_MainRenderView = nullptr;

	ReleaseRenderPasses();

	ReleaseGlobalShaderData();

	ReleaseRenderCore();
}

bool rf::Renderer::Load() {
	if (!m_RenderCore->Load()) {
		DFAssert(false, "Couldn't load RenderCore!");
		return false;
	}

	return true;
}

void rf::Renderer::Unload() {
	m_RenderCore->CompleteAllCommands();
	m_RenderCore->Unload();
}

void rf::Renderer::RenderFrame() {
	if (!CheckWindowSize()) {
		return;
	}

	rf::RenderContext context;
	if (!m_RenderCore->BeginFrame(context)) {
		return;
	}

	UpdateParams(context.m_FrameIndex);

	if (m_PresentRenderView && m_PresentRenderView->RenderFrame(*m_RenderCore, context)) {
		m_RenderCore->Present(m_PresentRenderView->GetOutput());
	} else {
		m_RenderCore->Present(rf::GlobalObjects::Get(ETexture::Black));
	}

	m_RenderCore->EndFrame();
}

void rf::Renderer::CompleteAllCommands() {
	m_RenderCore->CompleteAllCommands();
}

auto rf::Renderer::GetParamSetDefinitionManager()->rf::ParamSetDefinitionManager& {
	return m_ParamSetDefinitionManager;
}

auto rf::Renderer::GetParamSetDefinitionManager() const -> const rf::ParamSetDefinitionManager& {
	return m_ParamSetDefinitionManager;
}

void rf::Renderer::DestroyRenderView(rf::RenderView* renderView) {
	if (m_PresentRenderView == renderView) {
		m_PresentRenderView = nullptr;
	}

	df::RemoveUnordered(m_RenderViews, renderView);
	renderView->Release(*m_RenderCore);
	DFDelete renderView;
}

auto rf::Renderer::CreateTexture(const df::StringView& name, ETextureFormat format, uint32 width, uint32 height, void* data, uint32 dataSize)->TextureId {
	TextureId texture = m_RenderCore->CreateTexture(name, width, height, EImageUsageFlag::Texture, format);
	m_RenderCore->SetTextureData(texture, data, dataSize);

	return texture;
}

void rf::Renderer::DestroyTexture(TextureId texture) {
	m_RenderCore->DestroyTexture(texture);
}

auto rf::Renderer::CreateStaticBuffer(const df::StringView& name, const void* data, uint32 size, rf::BufferUsageFlags usageFlags, rf::EBufferAccessType accessType)->BufferId {
	BufferId buffer = m_RenderCore->CreateBuffer(name, size, usageFlags, accessType, true);
	m_RenderCore->SetBufferData(buffer, data, size);

	return buffer;
}

auto rf::Renderer::CreateDynamicBuffer(const df::StringView& name, uint32 size, rf::BufferUsageFlags usageFlags, rf::EBufferAccessType accessType)->BufferId {
	return m_RenderCore->CreateBuffer(name, size, usageFlags, accessType, false);
}

void rf::Renderer::SetBufferData(BufferId buffer, const void* data, uint32 size, uint32 offset /*= 0*/) {
	m_RenderCore->SetBufferData(buffer, data, size, offset);
}

void rf::Renderer::DestroyBuffer(BufferId buffer) {
	m_RenderCore->DestroyBuffer(buffer);
}

auto rf::Renderer::CreateMesh(uint32 vertexCount, uint32 indexCount)->MeshId {
	return m_RenderCore->CreateMesh(vertexCount, indexCount);
}

void rf::Renderer::SetMeshAttributeBuffer(MeshId mesh, uint32 attributeId, const void* data) {
	m_RenderCore->SetMeshAttributeBuffer(mesh, attributeId, data);
}

void rf::Renderer::SetMeshIndexBuffer(MeshId mesh, const uint32* data) {
	m_RenderCore->SetMeshIndexBuffer(mesh, data);
}

void rf::Renderer::DestroyMesh(MeshId mesh) {
	m_RenderCore->DestroyMesh(mesh);
}

auto rf::Renderer::CreateVertexShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::VertexShaderModuleId {
	return m_RenderCore->CreateVertexShaderModule(name, code);
}

auto rf::Renderer::CreateVertexShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::VertexShaderModuleId {
	return m_RenderCore->CreateVertexShaderModule(name, codeData, codeLength);
}

void rf::Renderer::DestroyVertexShaderModule(rf::VertexShaderModuleId shaderModule) {
	m_RenderCore->DestroyVertexShaderModule(shaderModule);
}

auto rf::Renderer::CreateFragmentShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::FragmentShaderModuleId {
	return m_RenderCore->CreateFragmentShaderModule(name, code);
}

auto rf::Renderer::CreateFragmentShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::FragmentShaderModuleId {
	return m_RenderCore->CreateFragmentShaderModule(name, codeData, codeLength);
}

void rf::Renderer::DestroyFragmentShaderModule(rf::FragmentShaderModuleId shaderModule) {
	m_RenderCore->DestroyFragmentShaderModule(shaderModule);
}

auto rf::Renderer::CreateComputeShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ComputeShaderModuleId {
	return m_RenderCore->CreateComputeShaderModule(name, code);
}

auto rf::Renderer::CreateComputeShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ComputeShaderModuleId {
	return m_RenderCore->CreateComputeShaderModule(name, codeData, codeLength);
}

void rf::Renderer::DestroyComputeShaderModule(rf::ComputeShaderModuleId shaderModule) {
	m_RenderCore->DestroyComputeShaderModule(shaderModule);
}

auto rf::Renderer::CreateGraphicsPipeline(const df::StringView& name, rf::VertexShaderModuleId vertexShader, rf::FragmentShaderModuleId fragmentShader)->rf::GraphicsPipelineId {
	return m_RenderCore->CreateGraphicsPipeline(name, vertexShader, fragmentShader);
}

void rf::Renderer::DestroyGraphicsPipeline(rf::GraphicsPipelineId pipeline) {
	m_RenderCore->DestroyGraphicsPipeline(pipeline);
}

auto rf::Renderer::CreateMaterialRule(const df::StringView& name)->rf::MaterialRuleId {
	return m_RenderCore->CreateMaterialRule(name);
}

void rf::Renderer::InitMaterialRule(rf::MaterialRuleId materialRule) {
	m_RenderCore->InitMaterialRule(materialRule);
}

void rf::Renderer::DestroyMaterialRule(rf::MaterialRuleId materialRule) {
	m_RenderCore->DestroyMaterialRule(materialRule);
}

auto rf::Renderer::CreateMaterial(rf::MaterialRuleId materialRule)->rf::MaterialId {
	return m_RenderCore->CreateMaterial(materialRule);
}

void rf::Renderer::DestroyMaterial(rf::MaterialId material) {
	m_RenderCore->DestroyMaterial(material);
}



bool rf::Renderer::InitRenderCore() {
	m_RenderCore = DFNew rf::RenderCore;
	if (!m_RenderCore->Init(m_Window)) {
		DFAssert(false, "Can't create render core!");
		return false;
	}

	rf::GlobalObjects::Create(*m_RenderCore);

	return true;
}

void rf::Renderer::ReleaseRenderCore() {
	rf::GlobalObjects::Destroy(*m_RenderCore);

	m_RenderCore->Release();
	DFDelete m_RenderCore;
	m_RenderCore = nullptr;
}

bool rf::Renderer::InitRendePasses() {
	for (uint32 i = 0; i < rf::RenderPassRegistry::TypeCount; ++i) {
		auto& entry = rf::RenderPassRegistry::Entries[i];

		entry.m_ParamSetDefinition->CreateRenderData(*m_RenderCore);

		df::Vector<rf::AttachmentDefinition> attachmentDefinitions;
		attachmentDefinitions.reserve(entry.m_OutputRenderTargetCount);

		for (uint32 oIdx = 0; oIdx < entry.m_OutputRenderTargetCount; ++oIdx) {
			const uint32 rtId = entry.m_OutputRenderTargets[oIdx];

			rf::AttachmentDefinition attachment = {};
			attachment.m_Format = rf::RenderTargetRegistry::Entries[rtId].m_Format;
			const bool isDepth = (
				attachment.m_Format == rf::ETextureFormat::D32_Float ||
				attachment.m_Format == rf::ETextureFormat::D24_Float_S8_UInt
				);
			if (isDepth) {
				attachment.m_InitialLayout = rf::EImageLayout::DepthStencilReadOnly;
				attachment.m_FinalLayout = rf::EImageLayout::DepthStencilReadOnly;
				attachment.m_ClearValue = { 0.0f, 0.0f };
			} else {
				attachment.m_InitialLayout = rf::EImageLayout::ColorReadOnly;
				attachment.m_FinalLayout = rf::EImageLayout::ColorReadOnly;
				attachment.m_ClearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
			}
			attachment.m_LoadOp = rf::EAttachmentLoadOp::Clear;
			attachment.m_StoreOp = rf::EAttachmentStoreOp::Store;
			attachmentDefinitions.emplace_back(attachment);
		}

		entry.m_RenderPass = m_RenderCore->CreateRenderPass(entry.m_Name, attachmentDefinitions);
	}

	return true;
}

void rf::Renderer::ReleaseRenderPasses() {
	for (uint32 i = 0; i < rf::RenderPassRegistry::TypeCount; ++i) {
		auto& entry = rf::RenderPassRegistry::Entries[i];

		m_RenderCore->DestroyRenderPass(entry.m_RenderPass);
		entry.m_RenderPass = nullptr;
	}
}

void rf::Renderer::InitGlobalShaderData() {
}

void rf::Renderer::ReleaseGlobalShaderData() {
	m_ParamSetDefinitionManager.DestroyRenderData(*m_RenderCore);
}

bool rf::Renderer::CheckWindowSize() {
	const uint32 width = m_Window->GetWidth();
	const uint32 height = m_Window->GetHeight();

	if (width == 0 || height == 0) {
		return false;
	}

	if (m_FramebufferWidth == width && m_FramebufferHeight == height) {
		return true;
	}

	m_FramebufferWidth = width;
	m_FramebufferHeight = height;

	m_RenderCore->CompleteAllCommands();

	if (!m_RenderCore->RecreateSwapchain(m_FramebufferWidth, m_FramebufferHeight)) {
		return false;
	}

	/*for (auto&& rv : m_RenderViews) {
		rv->OnScreenSizeChanged(m_FramebufferWidth, m_FramebufferHeight);
	}*/

	return true;
}

void rf::Renderer::CheckRenderPassFormats() {
	df::Vector<rf::PassId> testRenderPasses;

	for (int i = 1; i< int(rf::ETextureFormat::COUNT); ++i) {
		rf::AttachmentDefinition attachment = {};
		attachment.m_Format = rf::ETextureFormat(i);
		const bool isDepth = (
			attachment.m_Format == rf::ETextureFormat::D32_Float ||
			attachment.m_Format == rf::ETextureFormat::D24_Float_S8_UInt
			);

		if (isDepth) {
			attachment.m_InitialLayout = rf::EImageLayout::DepthStencilReadOnly;
			attachment.m_FinalLayout = rf::EImageLayout::DepthStencilReadOnly;
			attachment.m_ClearValue = { 0.0f, 0.0f };
		} else {
			attachment.m_InitialLayout = rf::EImageLayout::ColorReadOnly;
			attachment.m_FinalLayout = rf::EImageLayout::ColorReadOnly;
			attachment.m_ClearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		attachment.m_LoadOp = rf::EAttachmentLoadOp::Clear;
		attachment.m_StoreOp = rf::EAttachmentStoreOp::Store;

		testRenderPasses.push_back(m_RenderCore->CreateRenderPass(df::DebugName("Test"), { attachment }));
	}

	for (auto rp : testRenderPasses) {
		m_RenderCore->DestroyRenderPass(rp);
	}
}

void rf::Renderer::AddRenderView(rf::RenderView* renderView) {
	renderView->Init(*m_RenderCore);
	m_RenderViews.push_back(renderView);

	if (!m_PresentRenderView) {
		m_PresentRenderView = renderView;
	}
}