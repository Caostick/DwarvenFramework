#pragma once

#include <DwarfConstructorRender/VertexAttribute.h>

namespace {
	auto ToShaderType(rf::EVertexAttributeFormat fmt) -> rf::sg::EVariableType {
		const rf::EShaderInOutType attribType = rf::ToShaderInOutType(fmt);
		switch (attribType) {
		case rf::EShaderInOutType::Float:
			return rf::sg::EVariableType::Float;
		case rf::EShaderInOutType::Vec2:
			return rf::sg::EVariableType::Vec2;
		case rf::EShaderInOutType::Vec3:
			return rf::sg::EVariableType::Vec3;
		case rf::EShaderInOutType::Vec4:
			return rf::sg::EVariableType::Vec4;
		case rf::EShaderInOutType::Int:
			return rf::sg::EVariableType::Float;
		case rf::EShaderInOutType::IVec2:
			return rf::sg::EVariableType::Vec2;
		case rf::EShaderInOutType::IVec3:
			return rf::sg::EVariableType::Vec3;
		case rf::EShaderInOutType::IVec4:
			return rf::sg::EVariableType::Vec4;
		case rf::EShaderInOutType::UInt:
			return rf::sg::EVariableType::Float;
		case rf::EShaderInOutType::UIVec2:
			return rf::sg::EVariableType::Vec2;
		case rf::EShaderInOutType::UIVec3:
			return rf::sg::EVariableType::Vec3;
		case rf::EShaderInOutType::UIVec4:
			return rf::sg::EVariableType::Vec4;
		default:
			return rf::sg::EVariableType::Unknown;
		}
	}
}

template<typename SetupType>
rf::sg::TVertexShaderGenerator<SetupType>::TVertexShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager)
	: VertexShaderGenerator(paramSetDefinitionManager) {}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetGlobalParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetGlobalParameter<ParamType>(this);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetViewParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetViewParameter<ParamType>(this);
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetPassParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetPassParameter<RenderPassType, ParamType>(this);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetGlobalInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetGlobalInstanceBufferValue<ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetViewInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetViewInstanceBufferValue<ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TVertexShaderGenerator<SetupType>::GetPassInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetPassInstanceBufferValue<RenderPassType, ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename AttributeType>
auto rf::sg::TVertexShaderGenerator<SetupType>::RequestVertexAttribute()->const Variable& {
	static_assert(df::Contains<AttributeType, SetupType::VertexAttributes>::value, "VertexAttribute isn't declared in setup!");

	const auto attrIdx = rf::VertexAttributeTypeInfo<AttributeType>::Id;
	const auto arrtFmt = rf::VertexAttributeRegistry::Entries[attrIdx].Format;
	const auto attrName = rf::VertexAttributeRegistry::Entries[attrIdx].Name;
	const auto attrType = ToShaderType(arrtFmt);

	return RequestInput(attrType, attrName, false);
}