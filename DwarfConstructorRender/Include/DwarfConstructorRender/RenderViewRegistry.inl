#pragma once

template<typename T>
auto rf::RenderViewTypeInfo<T>::GetId()->uint32 {
	static uint32 id = Create();
	return id;
}

template<typename T>
auto rf::RenderViewTypeInfo<T>::Create()->uint32 {
	static_assert(std::is_base_of<rf::RenderView, T>::value, "Class is not child of RenderView!");
	const uint32 id = rf::RenderViewCommon::TypeCount++;
	return id;
}