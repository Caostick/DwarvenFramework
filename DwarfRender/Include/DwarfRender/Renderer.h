#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarfRender/VertexAttribute.h>

namespace df {
	class Mesh;
	class Buffer;
	class Texture;
	class RenderPass;
	class GraphicsPipeline;
	class ParameterSet;
	class CommandBuffer;
	class Window;
	class VertexAttributeHandler;
	class ParameterSetHandler;
	class SnippetProvider;
}

namespace df {
	class Renderer {
	public:
		virtual ~Renderer() {}

		virtual bool Init() = 0;
		virtual void Release() = 0;

		virtual void SetWindowSource(Window* window, Texture* texture) = 0;

		virtual auto CreateMesh()->Mesh* = 0;
		virtual void DestroyMesh(Mesh* mesh) = 0;

		virtual auto CreateBuffer()->Buffer* = 0;
		virtual void DestroyBuffer(Buffer* buffer) = 0;

		virtual auto CreateTexture()->Texture* = 0;
		virtual void DestroyTexture(Texture* texture) = 0;

		virtual auto CreateRenderPass()->RenderPass* = 0;
		virtual void DestroyRenderPass(RenderPass* renderPass) = 0;

		virtual auto RegisterVertexAttribute(const StringView& name, EVertexAttributeFormat format) -> const VertexAttribute* = 0;
		virtual auto FindVertexAttribute(const StringView& name) const -> const VertexAttribute* = 0;

		virtual auto CreateParameterSet(const StringView& className)->ParameterSet* = 0;
		virtual void DestroyParameterSet(ParameterSet* parameterSet) = 0;

		virtual void AddShaderInclude(const StringView& name, const StringView& content) = 0;

		virtual auto CreateGraphicsPipeline()->GraphicsPipeline* = 0;
		virtual void DestroyGraphicsPipeline(GraphicsPipeline* pipeline) = 0;

		virtual auto BeginFrame()->CommandBuffer* = 0;
		virtual void EndFrame() = 0;

		virtual auto GetSnippetProvider() const -> const SnippetProvider& = 0;
	};
}