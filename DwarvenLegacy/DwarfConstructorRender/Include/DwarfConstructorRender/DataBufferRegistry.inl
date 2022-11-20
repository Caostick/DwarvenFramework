#pragma once

template<typename T>
auto rf::DataBufferTypeInfo<T>::GetId()->uint32 {
	static uint32 id = Create();
	return id;
}

template<typename T>
auto rf::DataBufferTypeInfo<T>::Create()->uint32 {
	static_assert(std::is_base_of<rf::DataBuffer, T>::value, "Class is not child of DataBuffer!");
	const uint32 id = rf::DataBufferRegistry::TypeCount++;

	auto& entry = rf::DataBufferRegistry::Entries[id];

	entry.m_Name = T::Name;

	return id;
}