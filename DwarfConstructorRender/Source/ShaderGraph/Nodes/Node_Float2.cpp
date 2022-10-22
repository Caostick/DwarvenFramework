#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float2.h>
#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfConstructorRender/ShaderGraph/Variable.h>

#include <DwarfConstructorRender/ParamSetDeclarator.h>

void rf::sg::Node_Float2::MakeBindings() {
	Bind("Value", &m_Value);
	Bind({ "X", "R" }, &m_Value.X);
	Bind({ "Y", "G" }, &m_Value.Y);

	Bind("ParameterName", &m_ParameterName);
}

void rf::sg::Node_Float2::MakeVariable(PipelineGenerator& pipeline) {
	if (m_ParameterName.empty()) {
		m_Variable = &pipeline.MakeConstant(m_Value.X, m_Value.Y);
	} else {
		const auto& param = pipeline.GetLocalParameter(m_ParameterName);
		if (pipeline.IsMultiframed()) {
			m_Variable = &param;
		} else {
			m_Variable = &pipeline.NewVariable(param[0]);
		}
	}
}

void rf::sg::Node_Float2::AddParameter(ParamSetDeclarator& paramDeclarator) {
	paramDeclarator.DeclareVec2Parameter(m_ParameterName, m_Value);
}

bool rf::sg::Node_Float2::IsParameter() const {
	return !m_ParameterName.empty();
}