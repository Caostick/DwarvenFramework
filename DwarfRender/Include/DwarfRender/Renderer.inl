#pragma once

#include <DwarfRender/ParamSetDefinition.h>

#include <DwarvenCore/TypeList.h>
#include <DwarvenCore/New.h>

namespace {
	template<typename TL, typename Enable = void>
	struct VertexAttributeTypeInitializer;

	template<typename TL>
	struct VertexAttributeTypeInitializer<TL, typename std::enable_if<df::Length<TL>::value == 0>::type> {
		static void Init() {}
	};

	template<typename TL>
	struct VertexAttributeTypeInitializer<TL, typename std::enable_if<df::Length<TL>::value != 0>::type> {
	public:
		static void Init() {
			using VertexAttributeType = TL::Head;
			rf::VertexAttributeTypeInfo<VertexAttributeType>::GetId();
			VertexAttributeTypeInitializer<TL::Tail>::Init();
		}
	};



	template<typename TL, typename Enable = void>
	struct RenderPassTypeInitializer;

	template<typename TL>
	struct RenderPassTypeInitializer<TL, typename std::enable_if<df::Length<TL>::value == 0>::type> {
		static void Init(rf::Renderer&) {}
	};

	template<typename TL>
	struct RenderPassTypeInitializer<TL, typename std::enable_if<df::Length<TL>::value != 0>::type> {
	public:
		static void Init(rf::Renderer& renderer) {
			using RenderPassType = TL::Head;
			const uint32 id = rf::RenderPassTypeInfo<RenderPassType>::GetId();
			auto& entry = rf::RenderPassRegistry::Entries[id];

			const auto name = entry.m_Name;
			auto& paramsDefManager = renderer.GetParamSetDefinitionManager();

			entry.m_ParamSetDefinition = paramsDefManager.RequestDefinitionForPass(name);
			rf::ParamSetDeclarator decl(*entry.m_ParamSetDefinition);
			RenderPassType::InitParams(decl);
			entry.m_ParamSetDefinition->Build();

			RenderPassTypeInitializer<TL::Tail>::Init(renderer);
		}
	};



	template<typename TL, typename Enable = void>
	struct RenderViewPassConstructor;

	template<typename TL>
	struct RenderViewPassConstructor<TL, typename std::enable_if<df::Length<TL>::value == 0>::type> {
		static void Construct(rf::RenderView&) {}
	};

	template<typename TL>
	struct RenderViewPassConstructor<TL, typename std::enable_if<df::Length<TL>::value != 0>::type> {
	public:
		static void Construct(rf::RenderView& renderView) {
			using RenderPassType = typename TL::Head;
			renderView.AddRenderPass<RenderPassType>();
			RenderViewPassConstructor<typename TL::Tail>::Construct(renderView);
		}
	};
}

template<typename SetupType>
rf::TRenderer<SetupType>::~TRenderer() {

}

template<typename SetupType>
bool rf::TRenderer<SetupType>::Init(df::Window* window) {
	const bool result = Renderer::Init(window);

	VertexAttributeTypeInitializer<typename SetupType::VertexAttributes>::Init();

	RenderPassTypeInitializer<typename SetupType::RenderPasses>::Init(*this);
	InitRendePasses();

	rf::ParamSetDefinition* globalParamsDef = m_ParamSetDefinitionManager.RequestGlobalDefinition();
	rf::TParamSetDeclarator<SetupType::GlobalShaderParams>::Declare(*globalParamsDef);
	globalParamsDef->Build();
	globalParamsDef->CreateRenderData(*m_RenderCore);

	rf::ParamSetDefinition* viewParamsDef = m_ParamSetDefinitionManager.RequestViewDefinition();
	rf::TParamSetDeclarator<SetupType::ViewShaderParams>::Declare(*viewParamsDef);
	viewParamsDef->Build();
	viewParamsDef->CreateRenderData(*m_RenderCore);

	m_Params = DFNew rf::TParamSet<SetupType::GlobalShaderParams>(*globalParamsDef);
	m_Params->Init(*m_RenderCore, false);

	return result;
}

template<typename SetupType>
void rf::TRenderer<SetupType>::Release() {
	m_Params->Release(*m_RenderCore);
	DFDelete m_Params;

	rf::Renderer::Release();
}

template<typename SetupType>
auto rf::TRenderer<SetupType>::CreateShaderGraph() const->rf::sg::ShaderGraph* {
	static_assert(std::is_base_of<rf::sg::ShaderGraph, SetupType::ShaderGraphType>::value, "Class is not child of ShaderGraph!");

	auto shaderGraph = DFNew SetupType::ShaderGraphType();
	shaderGraph->Init();
	return shaderGraph;
}

template<typename SetupType>
template<typename RenderViewType, typename ...TArgs>
auto rf::TRenderer<SetupType>::CreateRenderView(TArgs&&... args)->RenderViewType* {
	static_assert(std::is_base_of<rf::RenderView, RenderViewType>::value, "Class is not child of RenderView!");
	RenderViewType* renderView = DFNew RenderViewType(std::forward<TArgs>(args)...);
	renderView->InitParams(*m_RenderCore, m_ParamSetDefinitionManager);

	using RenderPasses = typename RenderViewType::RenderPasses;
	if constexpr (!df::IsEmpty<RenderPasses>::value) {
		RenderViewPassConstructor<RenderPasses>::Construct(*renderView);
	}

	AddRenderView(renderView);
	return renderView;
}

template<typename SetupType>
template<typename AttributeType, typename T>
void rf::TRenderer<SetupType>::SetMeshAttributeBuffer(MeshId mesh, const T* data) {
	const auto attrId = rf::VertexAttributeTypeInfo<AttributeType>::Id;
	Renderer::SetMeshAttributeBuffer(mesh, attrId, data);
}

template<typename SetupType>
template<typename ParamType, typename DataType>
void rf::TRenderer<SetupType>::SetGlobalConstant(const DataType& value) {
	DFAssert(m_Params != nullptr, "Global params weren't initialized!");
	m_Params->SetConstant<ParamType, DataType>(value);
}

template<typename SetupType>
void rf::TRenderer<SetupType>::UpdateParams(uint32 frameIdx) {
	m_Params->Update(frameIdx);
}