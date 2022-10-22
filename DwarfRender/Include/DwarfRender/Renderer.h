#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarfRender/VertexAttribute.h>

namespace df {
	class Mesh;
	class Buffer;
	class Texture;
	class RenderPass;
	class Pipeline;
	class ParameterSet;
	class CommandBuffer;
}

namespace df {
	class Renderer {
	public:
		virtual ~Renderer() {}

		virtual bool Init() = 0;
		virtual void Release() = 0;

		virtual auto CreateMesh()->df::Mesh* = 0;
		virtual void DestroyMesh(df::Mesh* mesh) = 0;

		virtual auto CreateBuffer()->df::Buffer* = 0;
		virtual void DestroyBuffer(df::Buffer* buffer) = 0;

		virtual auto CreateTexture()->df::Texture* = 0;
		virtual void DestroyTexture(df::Texture* texture) = 0;

		virtual auto CreateRenderPass()->df::RenderPass* = 0;
		virtual void DestroyRenderPass(df::RenderPass* renderPass) = 0;

		virtual auto RegisterVertexAttribute(const df::StringView& name, df::EVertexAttributeFormat format, uint32 index) -> const df::VertexAttribute* = 0;
		virtual auto FindVertexAttribute(const df::StringView& name) const -> const df::VertexAttribute* = 0;

		virtual auto CreateParameterSet(const df::StringView& name)->df::ParameterSet* = 0;
		virtual void DestroyParameterSet(df::ParameterSet* parameterSet) = 0;

		virtual void AddShaderInclude(const df::StringView& name, const df::StringView& content) = 0;

		virtual auto CreatePipeline()->df::Pipeline* = 0;
		virtual void DestroyPipeline(df::Pipeline* pipeline) = 0;

		virtual void Update() = 0;

		virtual auto BeginFrame()->df::CommandBuffer* = 0;
		virtual void EndFrame() = 0;

		virtual void Present(df::Texture* texture) = 0;
	};
}