#pragma once

#include <DwarvenCore/Assert.h>

template<typename T>
auto rf::VertexAttributeTypeInfo<T>::GetId()->uint32 {
	static uint32 id = Create();
	return id;
}

template<typename T>
auto rf::VertexAttributeTypeInfo<T>::Create()->uint32 {
	static_assert(std::is_base_of<rf::VertexAttribute, T>::value, "Class is not child of VertexAttribute!");

	const uint32 id = rf::VertexAttributeRegistry::TypeCount++;
	const uint32 sizeOf = rf::ToAttributeSize(T::Format);

	DFAssert(sizeof(T::DataType) == sizeOf, "Vertex attribute data size doesn't match format size!");

	rf::VertexAttributeRegistry::Entries[id].Name = T::Name;
	rf::VertexAttributeRegistry::Entries[id].Format = T::Format;
	rf::VertexAttributeRegistry::Entries[id].SizeOf = sizeOf;

	return id;
}