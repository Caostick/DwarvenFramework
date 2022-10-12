#include <DwarfRender/ShaderGraph/VertexShaderGenerator.h>

rf::sg::VertexShaderGenerator::VertexShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager)
	: ShaderGenerator(paramSetDefinitionManager) {}

auto rf::sg::VertexShaderGenerator::GetVertexOutput() -> Variable& {
	sg::Variable* variable = m_Variables.Create(*this);
	variable->m_IsMutable = true;
	variable->m_IsMultiframed = false;
	variable->m_IsInitialized = true;
	variable->m_Type = EVariableType::Vec4;
	variable->m_Name = "gl_Position";
	variable->m_Expression = variable->m_Name;

	return *variable;
}

auto rf::sg::VertexShaderGenerator::RequestOutput(EVariableType type, const df::StringView& name, const Expression& value)->const Variable& {
	Variable& out = RequestOutput(type, name, value.IsMultiframed());
	out = value;
	return out;
}

auto rf::sg::VertexShaderGenerator::RequestOutput(EVariableType type, const df::StringView& name, bool isMultiframed /*= false*/)->Variable& {
	for (auto var : m_Outputs) {
		if (var->GetName() == name) {
			return *var;
		}
	}

	sg::Variable* variable = m_Variables.Create(*this);
	variable->m_IsMutable = true;
	variable->m_IsMultiframed = isMultiframed;
	variable->m_IsInitialized = true;
	variable->m_Type = type;
	variable->m_Name = name;
	variable->m_Expression = variable->m_Name + (variable->m_IsMultiframed ? "[#]" : "");

	m_Outputs.push_back(variable);
	return *variable;
}

auto rf::sg::VertexShaderGenerator::RequestInput(EVariableType type, const df::StringView& name, bool isMultiframed /*= false*/)->const Variable& {
	for (auto var : m_Inputs) {
		if (var->GetName() == name) {
			return *var;
		}
	}

	sg::Variable* variable = m_Variables.Create(*this);
	variable->m_IsMutable = true;
	variable->m_IsMultiframed = isMultiframed;
	variable->m_IsInitialized = true;
	variable->m_Type = type;
	variable->m_Name = name;
	variable->m_Expression = variable->m_Name + (variable->m_IsMultiframed ? "[#]" : "");

	m_Inputs.push_back(variable);
	return *variable;
}