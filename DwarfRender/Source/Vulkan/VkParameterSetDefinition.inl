#pragma once

template<typename T, df::EShaderConstantType ConstType>
auto vk::TShaderConstantDefinition<T, ConstType>::GetType() const->df::EShaderConstantType {
	return ConstType;
}

template<typename T, df::EShaderConstantType ConstType>
auto vk::TShaderConstantDefinition<T, ConstType>::GetSize() const->uint32 {
	return sizeof(T);
}