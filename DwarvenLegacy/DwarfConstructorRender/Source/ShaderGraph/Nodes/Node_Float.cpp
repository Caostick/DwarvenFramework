#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float.h>
#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfConstructorRender/ShaderGraph/Variable.h>

#include <DwarfConstructorRender/ParamSetDeclarator.h>

void rf::sg::Node_Float::MakeBindings() {
	Bind("Value", &m_Value);

	Bind("ParameterName", &m_ParameterName);
}

void rf::sg::Node_Float::MakeVariable(PipelineGenerator& pipeline) {
	if (m_ParameterName.empty()) {
		m_Variable = &pipeline.MakeConstant(m_Value);
	} else {
		const auto& param = pipeline.GetLocalParameter(m_ParameterName);
		if (pipeline.IsMultiframed()) {
			m_Variable = &param;
		} else {
			m_Variable = &pipeline.NewVariable(param[0]);
		}
	}
}

void rf::sg::Node_Float::AddParameter(ParamSetDeclarator& paramDeclarator) {
	paramDeclarator.DeclareFloatParameter(m_ParameterName, m_Value);
}

bool rf::sg::Node_Float::IsParameter() const {
	return !m_ParameterName.empty();
}