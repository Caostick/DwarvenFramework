#pragma once

#include <DwarfRender/Renderer.h>

#include "VkRenderCore.h"

namespace df {
	class Window;
}

namespace vk {
	class CommandBuffer;
}

namespace vk {
	class Renderer : public df::Renderer {
	public:
		Renderer(const df::Window& window);
		virtual ~Renderer() override;

		virtual bool Init() override;
		virtual void Release() override;

		virtual auto CreateMesh()->df::Mesh* override;
		virtual void DestroyMesh(df::Mesh* mesh) override;

		virtual auto CreateBuffer()->df::Buffer* override;
		virtual void DestroyBuffer(df::Buffer* buffer) override;

		virtual auto CreateTexture()->df::Texture* override;
		virtual void DestroyTexture(df::Texture* texture) override;

		virtual auto CreateRenderPass()->df::RenderPass* override;
		virtual void DestroyRenderPass(df::RenderPass* renderPass) override;

		virtual auto RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format, uint32 index) -> const df::VertexAttribute* override;
		virtual auto FindVertexAttribute(const df::StringView& name) const -> const df::VertexAttribute* override;

		virtual auto CreateParameterSet(const df::StringView& className)->df::ParameterSet* override;
		virtual void DestroyParameterSet(df::ParameterSet* parameterSet) override;

		virtual void AddShaderInclude(const df::StringView& name, const df::StringView& content) override;

		virtual auto CreatePipeline()->df::Pipeline* override;
		virtual void DestroyPipeline(df::Pipeline* pipeline) override;

		virtual auto BeginFrame()->df::CommandBuffer* override;
		virtual void EndFrame() override;

		virtual void Present(df::Texture* texture) override;

	private:
		vk::RenderCore m_RenderCore;
	};
}