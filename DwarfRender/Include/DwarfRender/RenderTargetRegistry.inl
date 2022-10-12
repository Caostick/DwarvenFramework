#pragma once

template<typename T>
auto rf::RenderTargetTypeInfo<T>::GetId()->uint32 {
	static uint32 id = Create();
	return id;
}

template<typename T>
auto rf::RenderTargetTypeInfo<T>::Create()->uint32 {
	static_assert(std::is_base_of<rf::RenderTarget, T>::value, "Class is not child of RenderTarget!");
	const uint32 id = rf::RenderTargetRegistry::TypeCount++;

	auto& entry = rf::RenderTargetRegistry::Entries[id];

	entry.m_Name = T::Name;
	entry.m_Format = T::Format;

	return id;
}