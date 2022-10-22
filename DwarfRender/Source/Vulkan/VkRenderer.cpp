#include "VkRenderer.h"

#include "VkMesh.h"
#include "VkBuffer.h"
#include "VkTexture.h"
#include "VkParameterSet.h"
#include "VkParameterSetDefinition.h"
#include "VkRenderPass.h"
#include "VkPipeline.h"
#include "VkCommandBuffer.h"
#include "VkRenderContext.h"

#include <DwarvenCore/New.h>

vk::Renderer::Renderer(const df::Window& window)
	: m_RenderCore(window) {
}

vk::Renderer::~Renderer() {

}

bool vk::Renderer::Init() {
	if (!m_RenderCore.Init()) {
		return false;
	}

	if (!m_RenderCore.Load()) {
		return false;
	}

	return true;
}

void vk::Renderer::Release() {
	m_RenderCore.CompleteAllCommands();
	m_RenderCore.Unload();
	m_RenderCore.Release();
}

auto vk::Renderer::CreateMesh()->df::Mesh* {
	return DFNew vk::Mesh;
}

void vk::Renderer::DestroyMesh(df::Mesh* mesh) {
	DFDelete mesh;
}

auto vk::Renderer::CreateBuffer()->df::Buffer* {
	return DFNew vk::Buffer;
}

void vk::Renderer::DestroyBuffer(df::Buffer* buffer) {
	DFDelete buffer;
}

auto vk::Renderer::CreateTexture()->df::Texture* {
	return DFNew vk::Texture;
}

void vk::Renderer::DestroyTexture(df::Texture* texture) {
	DFDelete texture;
}

auto vk::Renderer::CreateRenderPass()->df::RenderPass* {
	return m_RenderCore.CreateRenderPass();
}

void vk::Renderer::DestroyRenderPass(df::RenderPass* renderPass) {
	m_RenderCore.DestroyRenderPass(static_cast<vk::RenderPass*>(renderPass));
}

auto vk::Renderer::RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format, uint32 index) -> const df::VertexAttribute* {
	return m_RenderCore.RegisterVertexAttribute(name, format, index);
}

auto vk::Renderer::FindVertexAttribute(const df::StringView& name) const -> const df::VertexAttribute* {
	return m_RenderCore.FindVertexAttribute(name);
}

auto vk::Renderer::CreateParameterSet(const df::StringView& name)->df::ParameterSet* {
	return m_RenderCore.CreateParameterSet(name);
}

void vk::Renderer::DestroyParameterSet(df::ParameterSet* parameterSet) {
	m_RenderCore.DestroyParameterSet(static_cast<vk::ParameterSet*>(parameterSet));
}

void vk::Renderer::AddShaderInclude(const df::StringView& name, const df::StringView& content) {
	m_RenderCore.AddShaderInclude(name, content);
}

auto vk::Renderer::CreatePipeline()->df::Pipeline* {
	return m_RenderCore.CreatePipeline();
}

void vk::Renderer::DestroyPipeline(df::Pipeline* pipeline) {
	m_RenderCore.DestroyPipeline(static_cast<vk::Pipeline*>(pipeline));
}

void vk::Renderer::Update() {

}

auto vk::Renderer::BeginFrame()->df::CommandBuffer* {
	vk::RenderContext context = {};

	m_RenderCore.BeginFrame(context);

	return context.m_CommandBuffer;
}

void vk::Renderer::EndFrame() {
	m_RenderCore.EndFrame();
}

void vk::Renderer::Present(df::Texture* texture) {
	m_RenderCore.Present(static_cast<vk::Texture*>(texture));
}