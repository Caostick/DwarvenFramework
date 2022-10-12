#pragma once

#include <DwarfRender/Types.h>

namespace {
	auto ToShaderType(rf::ETextureFormat fmt) -> rf::sg::EVariableType {
		switch (fmt) {
		case rf::ETextureFormat::D32_Float:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::D24_Float_S8_UInt:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R8_UNorm:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R16_UNorm:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R8G8_UNorm:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R8G8B8A8_UNorm:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::A2R10G10B10_UNorm:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R16_Float:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R32_Float:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R16G16_Float:
			return rf::sg::EVariableType::Vec4;
		case rf::ETextureFormat::R16G16B16A16_Float:
			return rf::sg::EVariableType::Vec4;
		default:
			return rf::sg::EVariableType::Unknown;
		}
	}
}

template<typename SetupType>
rf::sg::TFragmentShaderGenerator<SetupType>::TFragmentShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager)
	: FragmentShaderGenerator(paramSetDefinitionManager) {}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetGlobalParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetGlobalParameter<ParamType>(this);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetViewParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetViewParameter<ParamType>(this);
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetPassParameter()->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetPassParameter<RenderPassType, ParamType>(this);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetGlobalInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetGlobalInstanceBufferValue<ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetViewInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetViewInstanceBufferValue<ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename RenderPassType, typename ParamType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::GetPassInstanceBufferValue(uint32 mul /*= 1*/, uint32 add /*= 0*/)->const Variable& {
	return TParameterVariableGenerator<SetupType>::GetPassInstanceBufferValue<RenderPassType, ParamType>(this, mul, add);
}

template<typename SetupType>
template<typename RenderPassType, typename RenderTargetType>
auto rf::sg::TFragmentShaderGenerator<SetupType>::RequestOutput()->Variable& {
	static_assert(df::Contains<RenderPassType, SetupType::RenderPasses>::value, "RenderPass isn't declared in setup!");
	static_assert(df::Contains<RenderTargetType, SetupType::RenderTargets>::value, "RenderTarget isn't declared in setup!");
	static_assert(df::Contains<RenderTargetType, RenderPassType::Outputs>::value, "RenderPass doesn't contain given RenderTarget!");

	const auto rtIdx = rf::RenderTargetTypeInfo<RenderTargetType>::Id;
	const EVariableType type = ToShaderType(rf::RenderTargetRegistry::Entries[rtIdx].m_Format);
	const char* name = rf::RenderTargetRegistry::Entries[rtIdx].m_Name;

	return FragmentShaderGenerator::RequestOutput(type, name);
}