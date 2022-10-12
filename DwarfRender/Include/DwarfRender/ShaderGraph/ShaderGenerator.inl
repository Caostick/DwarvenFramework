#pragma once

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetGlobalParameter(ShaderGenerator* shaderGenerator)->const Variable& {
	static_assert(std::is_base_of<rf::ConstantParameter, ParamType>::value, "Parameter is not derived from ConstantParameter!");
	static_assert(df::Contains<ParamType, SetupType::GlobalShaderParams>::value, "Global parameters doesn't contain given parameter!");

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, true);
	variable.m_Name = ParamType::Name;
	variable.m_Expression = "GlobalUBO[#]." + variable.m_Name;

	return variable;
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetViewParameter(ShaderGenerator* shaderGenerator)->const Variable& {
	static_assert(std::is_base_of<rf::ConstantParameter, ParamType>::value, "Parameter is not derived from ConstantParameter!");
	static_assert(df::Contains<ParamType, SetupType::ViewShaderParams>::value, "View parameters doesn't contain given parameter!");

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, true);
	variable.m_Name = ParamType::Name;
	variable.m_Expression = "ViewUBO[#]." + variable.m_Name;

	return variable;
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetPassParameter(ShaderGenerator* shaderGenerator)->const Variable& {
	static_assert(std::is_base_of<rf::ConstantParameter, ParamType>::value, "Parameter is not derived from ConstantParameter!");
	static_assert(df::Contains<ParamType, RenderPassType::Params>::value, "RenderPass doesn't contain given parameter!");

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, true);
	variable.m_Name = ParamType::Name;
	variable.m_Expression = df::String(rf::RenderPassRegistry::Entries[RenderPassType::Id].m_Name) + "UBO[#]." + variable.m_Name;

	return variable;
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetGlobalInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	static_assert(std::is_base_of<rf::BufferParameter, ParamType>::value, "Parameter is not derived from BufferParameter!");
	static_assert(df::Contains<ParamType, SetupType::GlobalShaderParams>::value, "Global parameters doesn't contain given parameter!");

	const bool isMultiframed = ParamType::Multiframed;
	const df::String strName = ParamType::Name;
	char expressionBuffer[128];

	if (isMultiframed) {
		sprintf_s(expressionBuffer, "%s[#].data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	} else {
		sprintf_s(expressionBuffer, "%s.data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	}

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, isMultiframed);
	variable.m_Name = strName;
	variable.m_Expression = expressionBuffer;

	return variable;
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetViewInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	static_assert(std::is_base_of<rf::BufferParameter, ParamType>::value, "Parameter is not derived from BufferParameter!");
	static_assert(df::Contains<ParamType, SetupType::ViewShaderParams>::value, "View parameters doesn't contain given parameter!");

	const bool isMultiframed = ParamType::Multiframed;
	const df::String strName = ParamType::Name;
	char expressionBuffer[128];

	if (isMultiframed) {
		sprintf_s(expressionBuffer, "%s[#].data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	} else {
		sprintf_s(expressionBuffer, "%s.data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	}

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, isMultiframed);
	variable.m_Name = strName;
	variable.m_Expression = expressionBuffer;

	return variable;
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::GetPassInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	static_assert(std::is_base_of<rf::BufferParameter, ParamType>::value, "Parameter is not derived from BufferParameter!");
	static_assert(df::Contains<ParamType, RenderPassType::Params>::value, "RenderPass doesn't contain given parameter!");

	const bool isMultiframed = ParamType::Multiframed;
	const df::String strName = ParamType::Name;
	char expressionBuffer[128];

	if (isMultiframed) {
		sprintf_s(expressionBuffer, "%s[#].data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	} else {
		sprintf_s(expressionBuffer, "%s.data[gl_InstanceIndex * %d + %d]", strName.c_str(), mul, add);
	}

	Variable& variable = CreateVariable<ParamType>(shaderGenerator, isMultiframed);
	variable.m_Name = strName;
	variable.m_Expression = expressionBuffer;

	return variable;
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TParameterVariableGenerator<SetupType>::CreateVariable(ShaderGenerator* shaderGenerator, bool isMultiframed) -> Variable& {
	Variable* variable = shaderGenerator->m_Variables.Create(*shaderGenerator);
	variable->m_IsInitialized = true;
	variable->m_IsMultiframed = isMultiframed;
	variable->m_IsMutable = false;

	if constexpr (std::is_base_of<rf::FloatParameter, ParamType>::value) {
		variable->m_Type = EVariableType::Float;
	}
	if constexpr (std::is_base_of<rf::Vec2Parameter, ParamType>::value) {
		variable->m_Type = EVariableType::Vec2;
	}
	if constexpr (std::is_base_of<rf::Vec3Parameter, ParamType>::value) {
		variable->m_Type = EVariableType::Vec3;
	}
	if constexpr (std::is_base_of<rf::Vec4Parameter, ParamType>::value) {
		variable->m_Type = EVariableType::Vec4;
	}
	if constexpr (std::is_base_of<rf::Mat3Parameter, ParamType>::value) {
		variable->m_Type = EVariableType::Mat3;
	}
	if constexpr (std::is_base_of<rf::Mat4Parameter, ParamType>::value) {
		variable->m_Type = EVariableType::Mat4;
	}
	if constexpr (std::is_base_of<rf::FloatBufferParameter, ParamType>::value) {
		variable->m_Type = EVariableType::Float;
	}
	if constexpr (std::is_base_of<rf::Vec2BufferParameter, ParamType>::value) {
		variable->m_Type = EVariableType::Vec2;
	}
	if constexpr (std::is_base_of<rf::Vec4BufferParameter, ParamType>::value) {
		variable->m_Type = EVariableType::Vec4;
	}

	return *variable;
}