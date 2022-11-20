#pragma once

template<typename SetupType>
rf::sg::TGraphicsPipelineGenerator<SetupType>::TGraphicsPipelineGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager) 
	: m_VertexShader(paramSetDefinitionManager)
	, m_FragmentShader(paramSetDefinitionManager) {
}

template<typename SetupType>
auto rf::sg::TGraphicsPipelineGenerator<SetupType>::GetVertexShader()->VertexShaderGenerator& {
	return m_VertexShader;
}

template<typename SetupType>
auto rf::sg::TGraphicsPipelineGenerator<SetupType>::GetFragmentShader()->FragmentShaderGenerator& {
	return m_FragmentShader;
}