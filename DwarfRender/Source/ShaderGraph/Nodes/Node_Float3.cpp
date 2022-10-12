#include <DwarfRender/ShaderGraph/Nodes/Node_Float3.h>
#include <DwarfRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfRender/ShaderGraph/Variable.h>

#include <DwarfRender/ParamSetDeclarator.h>

void rf::sg::Node_Float3::MakeBindings() {
	Bind("Value", &m_Value);
	Bind({ "X", "R" }, &m_Value.X);
	Bind({ "Y", "G" }, &m_Value.Y);
	Bind({ "Z", "B" }, &m_Value.Z);

	Bind("ParameterName", &m_ParameterName);
}

void rf::sg::Node_Float3::MakeVariable(PipelineGenerator& pipeline) {
	if (m_ParameterName.empty()) {
		m_Variable = &pipeline.MakeConstant(m_Value.X, m_Value.Y, m_Value.Z);
	} else {
		const auto& param = pipeline.GetLocalParameter(m_ParameterName);
		if (pipeline.IsMultiframed()) {
			m_Variable = &param;
		} else {
			m_Variable = &pipeline.NewVariable(param[0]);
		}
	}
}

void rf::sg::Node_Float3::AddParameter(ParamSetDeclarator& paramDeclarator) {
	paramDeclarator.DeclareVec3Parameter(m_ParameterName, m_Value);
}

bool rf::sg::Node_Float3::IsParameter() const {
	return !m_ParameterName.empty();
}