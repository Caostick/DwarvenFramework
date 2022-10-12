#include <DwarfRender/ShaderGraph/FragmentShaderGenerator.h>

rf::sg::FragmentShaderGenerator::FragmentShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager)
	: ShaderGenerator(paramSetDefinitionManager) {}

auto rf::sg::FragmentShaderGenerator::RequestInput(EVariableType type, const df::StringView& name, bool isMultiframed /*= false*/)->const Variable& {
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

auto rf::sg::FragmentShaderGenerator::RequestOutput(EVariableType type, const df::StringView& name)->Variable& {
	for (auto var : m_Outputs) {
		if (var->GetName() == name) {
			return *var;
		}
	}

	sg::Variable* variable = m_Variables.Create(*this);
	variable->m_IsMutable = true;
	variable->m_IsMultiframed = false;
	variable->m_IsInitialized = true;
	variable->m_Type = type;
	variable->m_Name = name;
	variable->m_Expression = variable->m_Name + (variable->m_IsMultiframed ? "[#]" : "");

	m_Outputs.push_back(variable);
	return *variable;
}