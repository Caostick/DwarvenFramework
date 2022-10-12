#pragma once

template<typename T, rf::EShaderParameterType ConstType>
auto rf::TShaderConstantDefinition<T, ConstType>::GetType() const->rf::EShaderParameterType {
	return ConstType;
}

template<typename T, rf::EShaderParameterType ConstType>
auto rf::TShaderConstantDefinition<T, ConstType>::GetSize() const->uint32 {
	return sizeof(T);
}

template<typename TL>
void rf::TParamSetDeclarator<TL, typename std::enable_if<df::Length<TL>::value == 0>::type>::Declare(ParamSetDefinition&) {

}

template<typename TL>
void rf::TParamSetDeclarator<TL, typename std::enable_if<df::Length<TL>::value != 0>::type>::Declare(ParamSetDefinition& params) {
	using ShaderParamType = TL::Head;
	static_assert(std::is_base_of<rf::ShaderParameter, ShaderParamType>::value,
		"ParamSet should contain only ShaderParameter derived classes!"
		);

	if constexpr (std::is_base_of<rf::FloatParameter, ShaderParamType>::value) {
		params.DeclareFloatParameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::Vec2Parameter, ShaderParamType>::value) {
		params.DeclareVec2Parameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::Vec3Parameter, ShaderParamType>::value) {
		params.DeclareVec3Parameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::Vec4Parameter, ShaderParamType>::value) {
		params.DeclareVec4Parameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::Mat3Parameter, ShaderParamType>::value) {
		params.DeclareMat3Parameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::Mat4Parameter, ShaderParamType>::value) {
		params.DeclareMat4Parameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::TextureParameter, ShaderParamType>::value) {
		params.DeclareTextureParameter(ShaderParamType::Name, ShaderParamType::DefaultValue);
	}

	if constexpr (std::is_base_of<rf::FloatBufferParameter, ShaderParamType>::value) {
		params.DeclareBufferParameter(ShaderParamType::Name, EShaderParameterType::Float, ShaderParamType::Multiframed);
	}

	if constexpr (std::is_base_of<rf::Vec2BufferParameter, ShaderParamType>::value) {
		params.DeclareBufferParameter(ShaderParamType::Name, EShaderParameterType::Vec2, ShaderParamType::Multiframed);
	}

	if constexpr (std::is_base_of<rf::Vec4BufferParameter, ShaderParamType>::value) {
		params.DeclareBufferParameter(ShaderParamType::Name, EShaderParameterType::Vec4, ShaderParamType::Multiframed);
	}

	TParamSetDeclarator<TL::Tail>::Declare(params);
}