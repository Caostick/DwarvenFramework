#include <DwarfRender/ShaderGraph/GraphicsPipelineGenerator.h>

rf::sg::GraphicsPipelineGenerator::~GraphicsPipelineGenerator() {
}

void rf::sg::GraphicsPipelineGenerator::Init() {
	m_Pass = EShaderPass::Fragment;
}

void rf::sg::GraphicsPipelineGenerator::Release() {
}

bool rf::sg::GraphicsPipelineGenerator::Build(rf::sg::ShaderGraph& /*shaderGraph*/) {
	return false;
}

void rf::sg::GraphicsPipelineGenerator::Reset(rf::ParamSetDefinition* parameters) {
	GetVertexShader().Reset(parameters);
	GetFragmentShader().Reset(parameters);
}

void rf::sg::GraphicsPipelineGenerator::SetPass(EShaderPass pass) {
	m_Pass = pass;

	if (m_Pass == EShaderPass::Vertex) {
		m_CurrentShader = &GetVertexShader();
	} else {
		m_CurrentShader = &GetFragmentShader();
	}
}