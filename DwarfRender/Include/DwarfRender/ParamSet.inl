#pragma once

template<typename ParamList>
rf::TParamSet<ParamList>::TParamSet(const ParamSetDefinition& definition)
	: ParamSet(definition) {
}

template<typename ParamList>
template<typename ParamType, typename DataType>
void rf::TParamSet<ParamList>::SetConstant(const DataType& value) {
	static_assert(std::is_base_of<rf::ConstantParameter, ParamType>::value, "Type should be a child of ConstantParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	if constexpr (std::is_base_of<rf::FloatParameter, ParamType>::value) {
		SetFloat(ParamType::Name, value);
	}

	if constexpr (std::is_base_of<rf::Vec2Parameter, ParamType>::value) {
		SetVec2(ParamType::Name, value);
	}

	if constexpr (std::is_base_of<rf::Vec3Parameter, ParamType>::value) {
		SetVec3(ParamType::Name, value);
	}

	if constexpr (std::is_base_of<rf::Vec4Parameter, ParamType>::value) {
		SetVec4(ParamType::Name, value);
	}

	if constexpr (std::is_base_of<rf::Mat3Parameter, ParamType>::value) {
		SetMat3(ParamType::Name, value);
	}

	if constexpr (std::is_base_of<rf::Mat4Parameter, ParamType>::value) {
		SetMat4(ParamType::Name, value);
	}
}

template<typename ParamList>
template<typename ParamType, typename DataType>
auto rf::TParamSet<ParamList>::GetConstant() const->DataType {
	static_assert(std::is_base_of<rf::ConstantParameter, ParamType>::value, "Type should be a child of ConstantParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	if constexpr (std::is_base_of<rf::FloatParameter, ParamType>::value) {
		return GetFloat(ParamType::Name);
	}

	if constexpr (std::is_base_of<rf::Vec2Parameter, ParamType>::value) {
		return GetVec2(ParamType::Name);
	}

	if constexpr (std::is_base_of<rf::Vec3Parameter, ParamType>::value) {
		return GetVec3(ParamType::Name);
	}

	if constexpr (std::is_base_of<rf::Vec4Parameter, ParamType>::value) {
		return GetVec4(ParamType::Name);
	}

	if constexpr (std::is_base_of<rf::Mat3Parameter, ParamType>::value) {
		return GetMat3(ParamType::Name);
	}

	if constexpr (std::is_base_of<rf::Mat4Parameter, ParamType>::value) {
		return GetMat4(ParamType::Name);
	}
}

template<typename ParamList>
template<typename ParamType>
void rf::TParamSet<ParamList>::SetBuffer(rf::BufferId buffer, bool isHistory /*= false*/) {
	static_assert(std::is_base_of<rf::BufferParameter, ParamType>::value, "Type should be a child of BufferParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	SetBuffer(ParamType::Name, buffer, isHistory);
}

template<typename ParamList>
template<typename ParamType>
auto rf::TParamSet<ParamList>::GetBuffer(bool isHistory /*= false*/) const->rf::BufferId {
	static_assert(std::is_base_of<rf::BufferParameter, ParamType>::value, "Type should be a child of BufferParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	return GetBuffer(ParamType::Name, isHistory);
}

template<typename ParamList>
template<typename ParamType>
void rf::TParamSet<ParamList>::SetTexture(rf::TextureId texture) {
	static_assert(std::is_base_of<rf::TextureParameter, ParamType>::value, "Type should be a child of TextureParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	SetTexture(ParamType::Name, texture);
}

template<typename ParamList>
template<typename ParamType>
auto rf::TParamSet<ParamList>::GetTexture() const->rf::TextureId {
	static_assert(std::is_base_of<rf::TextureParameter, ParamType>::value, "Type should be a child of TextureParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	return GetTexture(ParamType::Name);
}

template<typename ParamList>
template<typename ParamType>
void rf::TParamSet<ParamList>::SetSampler(rf::SamplerId sampler) {
	static_assert(std::is_base_of<rf::TextureParameter, ParamType>::value, "Type should be a child of TextureParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	SetSampler(ParamType::Name, sampler);
}

template<typename ParamList>
template<typename ParamType>
auto rf::TParamSet<ParamList>::GetSampler() const->rf::SamplerId {
	static_assert(std::is_base_of<rf::TextureParameter, ParamType>::value, "Type should be a child of TextureParameter!");
	static_assert(df::Contains<ParamType, ParamList>::value, "Parameter is not declared in parameter set definition!");

	return GetTexture(ParamType::Name);
}