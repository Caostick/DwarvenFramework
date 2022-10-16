#pragma once

#include <DwarfRender/ParamSetDefinitionManager.h>
#include <DwarfRender/ObjectId.h>
#include <DwarfRender/ParamSet.h>
#include <DwarfRender/Types.h>

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/Math/Math.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>
#include <DwarvenCore/New.h>

namespace df {
	class Window;
}

namespace rf {
	class RenderCore;
	class ParamSetDeclarator;
	class ParamSetDefinition;

	namespace sg {
		class ShaderGraph;
	}
}

namespace rf {
	class Renderer {
	public:
		Renderer();
		virtual ~Renderer();

		bool Init(df::Window* window);
		void Release();

		virtual auto CreateShaderGraph() const->rf::sg::ShaderGraph* = 0;

		bool Load();
		void Unload();

		void RenderFrame();
		void CompleteAllCommands();

		auto GetParamSetDefinitionManager()->rf::ParamSetDefinitionManager&;
		auto GetParamSetDefinitionManager() const -> const rf::ParamSetDefinitionManager&;

		void DestroyRenderView(rf::RenderView* renderView);

		auto CreateTexture(const df::StringView& name, ETextureFormat format, uint32 width, uint32 height, void* data, uint32 dataSize)->TextureId;
		void DestroyTexture(TextureId texture);

		auto CreateStaticBuffer(const df::StringView& name, const void* data, uint32 size, rf::BufferUsageFlags usageFlags, rf::EBufferAccessType accessType)->BufferId;
		auto CreateDynamicBuffer(const df::StringView& name, uint32 size, rf::BufferUsageFlags usageFlags, rf::EBufferAccessType accessType)->BufferId;
		void SetBufferData(BufferId buffer, const void* data, uint32 size, uint32 offset = 0);
		void DestroyBuffer(BufferId buffer);

		auto CreateMesh(uint32 vertexCount, uint32 indexCount)->MeshId;
		void SetMeshAttributeBuffer(MeshId mesh, uint32 attributeId, const void* data);
		void SetMeshIndexBuffer(MeshId mesh, const uint32* data);
		void DestroyMesh(MeshId mesh);

		auto CreateVertexShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::VertexShaderModuleId;
		auto CreateVertexShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::VertexShaderModuleId;
		void DestroyVertexShaderModule(rf::VertexShaderModuleId shaderModule);

		auto CreateFragmentShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::FragmentShaderModuleId;
		auto CreateFragmentShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::FragmentShaderModuleId;
		void DestroyFragmentShaderModule(rf::FragmentShaderModuleId shaderModule);

		auto CreateComputeShaderModule(const df::StringView& name, const df::Vector<uint32>& code)->rf::ComputeShaderModuleId;
		auto CreateComputeShaderModule(const df::StringView& name, const uint32* codeData, uint32 codeLength)->rf::ComputeShaderModuleId;
		void DestroyComputeShaderModule(rf::ComputeShaderModuleId shaderModule);

		auto CreateGraphicsPipeline(const df::StringView& name, rf::VertexShaderModuleId vertexShader, rf::FragmentShaderModuleId fragmentShader)->rf::GraphicsPipelineId;
		void DestroyGraphicsPipeline(rf::GraphicsPipelineId pipeline);

		auto CreateMaterialRule(const df::StringView& name)->rf::MaterialRuleId;
		void InitMaterialRule(rf::MaterialRuleId materialRule);
		void DestroyMaterialRule(rf::MaterialRuleId materialRule);

		auto CreateMaterial(rf::MaterialRuleId materialRule)->rf::MaterialId;
		void DestroyMaterial(rf::MaterialId material);

	protected:
		bool InitRenderCore();
		void ReleaseRenderCore();

		bool InitRendePasses();
		void ReleaseRenderPasses();

		void InitGlobalShaderData();
		void ReleaseGlobalShaderData();

		bool CheckWindowSize();

		void CheckRenderPassFormats();

		void AddRenderView(rf::RenderView* renderView);

		virtual void UpdateParams(uint32 frameIdx) = 0;

	protected:
		df::Window* m_Window = nullptr;
		rf::RenderCore* m_RenderCore = nullptr;
		rf::RenderView* m_MainRenderView = nullptr;

		rf::ParamSetDefinitionManager m_ParamSetDefinitionManager;

		uint32 m_FramebufferWidth = 0;
		uint32 m_FramebufferHeight = 0;

		df::Vector<rf::RenderView*> m_RenderViews;
		rf::RenderView* m_PresentRenderView = nullptr;
	};

	template<typename SetupType>
	class TRenderer : public Renderer {
	public:
		~TRenderer();

		bool Init(df::Window* window);
		void Release();

		auto CreateShaderGraph() const->rf::sg::ShaderGraph* override;

		template<typename RenderViewType, typename ...TArgs>
		auto CreateRenderView(TArgs&&... args)->RenderViewType*;

		template<typename AttributeType, typename T = typename AttributeType::DataType>
		void SetMeshAttributeBuffer(MeshId mesh, const T* data);

		template<typename ParamType, typename DataType = typename ParamType::DataType>
		void SetGlobalConstant(const DataType& value);

	protected:
		void UpdateParams(uint32 frameIdx) override;

	protected:
		rf::TParamSet<typename SetupType::GlobalShaderParams>* m_Params = nullptr;
	};
}

#include <DwarfRender/Renderer.inl>