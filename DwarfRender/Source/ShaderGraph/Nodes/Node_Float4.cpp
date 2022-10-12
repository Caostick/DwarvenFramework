#include <DwarfRender/ShaderGraph/Nodes/Node_Float4.h>
#include <DwarfRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfRender/ShaderGraph/Variable.h>

#include <DwarfRender/ParamSetDeclarator.h>

void rf::sg::Node_Float4::MakeBindings() {
	Bind("Value", &m_Value);
	Bind({ "X", "R" }, &m_Value.X);
	Bind({ "Y", "G" }, &m_Value.Y);
	Bind({ "Z", "B" }, &m_Value.Z);
	Bind({ "W", "A" }, &m_Value.W);

	Bind("ParameterName", &m_ParameterName);
}

void rf::sg::Node_Float4::MakeVariable(PipelineGenerator& pipeline) {
	if (m_ParameterName.empty()) {
		m_Variable = &pipeline.MakeConstant(m_Value.X, m_Value.Y, m_Value.Z, m_Value.W);
	} else {
		const auto& param = pipeline.GetLocalParameter(m_ParameterName);
		if (pipeline.IsMultiframed()) {
			m_Variable = &param;
		} else {
			m_Variable = &pipeline.NewVariable(param[0]);
		}
	}
}

void rf::sg::Node_Float4::AddParameter(ParamSetDeclarator& paramDeclarator) {
	paramDeclarator.DeclareVec4Parameter(m_ParameterName, m_Value);
}

bool rf::sg::Node_Float4::IsParameter() const {
	return !m_ParameterName.empty();
}