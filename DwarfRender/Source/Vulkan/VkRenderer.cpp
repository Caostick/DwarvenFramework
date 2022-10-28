#include "VkRenderer.h"

#include "VkMesh.h"
#include "VkBuffer.h"
#include "VkTexture.h"
#include "VkParameterSet.h"
#include "VkParameterSetDefinition.h"
#include "VkRenderPass.h"
#include "VkGraphicsPipeline.h"
#include "VkCommandBuffer.h"

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
	return m_RenderCore.CreateMesh();
}

void vk::Renderer::DestroyMesh(df::Mesh* mesh) {
	m_RenderCore.DestroyMesh(static_cast<vk::Mesh*>(mesh));
}

auto vk::Renderer::CreateBuffer()->df::Buffer* {
	return m_RenderCore.CreateBuffer();
}

void vk::Renderer::DestroyBuffer(df::Buffer* buffer) {
	m_RenderCore.DestroyBuffer(static_cast<vk::Buffer*>(buffer));
}

auto vk::Renderer::CreateTexture()->df::Texture* {
	return m_RenderCore.CreateTexture();
}

void vk::Renderer::DestroyTexture(df::Texture* texture) {
	m_RenderCore.DestroyTexture(static_cast<vk::Texture*>(texture));
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

auto vk::Renderer::CreateParameterSet(const df::StringView& className)->df::ParameterSet* {
	return m_RenderCore.CreateParameterSet(className);
}

void vk::Renderer::DestroyParameterSet(df::ParameterSet* parameterSet) {
	m_RenderCore.DestroyParameterSet(static_cast<vk::ParameterSet*>(parameterSet));
}

void vk::Renderer::AddShaderInclude(const df::StringView& name, const df::StringView& content) {
	m_RenderCore.AddShaderInclude(name, content);
}

auto vk::Renderer::CreateGraphicsPipeline()->df::GraphicsPipeline* {
	return m_RenderCore.CreateGraphicsPipeline();
}

void vk::Renderer::DestroyGraphicsPipeline(df::GraphicsPipeline* pipeline) {
	m_RenderCore.DestroyGraphicsPipeline(static_cast<vk::GraphicsPipeline*>(pipeline));
}

auto vk::Renderer::BeginFrame()->df::CommandBuffer* {
	return m_RenderCore.BeginFrame();
}

void vk::Renderer::EndFrame() {
	m_RenderCore.EndFrame();
}

void vk::Renderer::Present(df::Texture* texture) {
	m_RenderCore.Present(static_cast<vk::Texture*>(texture));
}