#pragma once

#include <DwarvenCore/New.h>

template<typename RenderTargetType>
auto rf::RenderView::GetRenderTarget()->rf::TextureId {
	static_assert(std::is_base_of<rf::RenderTarget, RenderTargetType>::value, "Class is not child of RenderTarget!");
	
	const auto typeId = rf::RenderTargetTypeInfo<RenderTargetType>::Id;
	return m_RenderTargets[typeId];
}

template<typename DataBufferType>
auto rf::RenderView::GetDataBuffer()->rf::BufferId {
	static_assert(std::is_base_of<rf::DataBuffer, DataBufferType>::value, "Class is not child of DataBuffer!");
	static_assert(df::Contains<DataBufferType, DataBuffers>::value, "DataBuffer is not registered in RenderView!");

	const auto typeId = rf::DataBufferTypeInfo<DataBufferType>::Id;
	return m_DataBuffers[typeId];
}

template<typename RenderPassType, typename ...TArgs>
auto rf::RenderView::AddRenderPass(TArgs&&... args) -> RenderPassType* {
	static_assert(std::is_base_of<rf::RenderPass, RenderPassType>::value, "Class is not child of RenderPass!");

	const auto typeId = rf::RenderPassTypeInfo<RenderPassType>::Id;
	RenderPassType* renderPass = DFNew RenderPassType(std::forward<TArgs>(args)...);

	AddRenderPass(renderPass, typeId);
	return renderPass;
}

template<typename SetupType>
void rf::TRenderView<SetupType>::InitParams(rf::RenderCore& renderCore, rf::ParamSetDefinitionManager& paramsDefinitionManager) {
	rf::ParamSetDefinition* viewParamsDef = paramsDefinitionManager.RequestViewDefinition();
	m_Params = DFNew rf::TParamSet<typename SetupType::ViewShaderParams>(*viewParamsDef);
	m_Params->Init(renderCore, false);
}

template<typename SetupType>
template<typename ParamType, typename DataType>
void rf::TRenderView<SetupType>::SetViewConstant(const DataType& value) {
	DFAssert(m_Params != nullptr, "View params weren't initialized!");
	m_Params->SetConstant<ParamType, DataType>(value);
}

template<typename SetupType>
void rf::TRenderView<SetupType>::UpdateParams(uint32 frameIdx) {
	m_Params->Update(frameIdx);
}

template<typename SetupType>
void rf::TRenderView<SetupType>::ReleaseParams(rf::RenderCore& renderCore) {
	m_Params->Release(renderCore);
	DFDelete m_Params;
	m_Params = nullptr;
}