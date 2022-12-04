#pragma once

#include <DwarvenCore/Assert.h>

template <typename DataType>
df::TResource<DataType>::DataContainer::DataContainer(const StringView& guid, const StringView& path, const StringView& ext)
	: m_RefCount(0) 
	, m_GUID(String(guid))
	, m_Path(String(path)) 
	, m_Ext(String(ext)) {

}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetRefCount() const->int32 {
	return m_RefCount;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetGUID() const->const String& {
	return m_GUID;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetPath() const->const String& {
	return m_Path;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetExt() const->const String& {
	return m_Ext;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetData()->Data& {
	return m_Data;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::GetData() const -> const Data& {
	return m_Data;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::TakeOne() -> int32 {
	return ++m_RefCount;
}

template <typename DataType>
auto df::TResource<DataType>::DataContainer::FreeOne() -> int32 {
	return --m_RefCount;
}


template <typename DataType>
df::TResource<DataType>::TResource()
	: m_Data(nullptr) {
}

template <typename DataType>
df::TResource<DataType>::TResource(DataContainer* data)
	: m_Data(data) {
	m_Data->TakeOne();
}

template <typename DataType>
df::TResource<DataType>::TResource(const TResource &other)
	: m_Data(other.m_Data) {
	m_Data->TakeOne();
}

template <typename DataType>
df::TResource<DataType>::TResource(TResource &&other)
	: m_Data(other.m_Data) {
	other.m_Data = nullptr;
}

template <typename DataType>
df::TResource<DataType>::~TResource() {
	if (m_Data) {
		m_Data->FreeOne();
		m_Data = nullptr;
	}
}

template <typename DataType>
auto df::TResource<DataType>::operator=(const TResource &other) -> TResource& {
	m_Data = other.m_Data;
	m_Data->TakeOne();

	return *this;
}

template <typename DataType>
auto df::TResource<DataType>::operator=(TResource &&other) -> TResource& {
	m_Data = other.m_Data;
	other.m_Data = nullptr;

	return *this;
}

template <typename DataType>
auto df::TResource<DataType>::GetContainer() -> DataContainer* {
	return m_Data;
}

template <typename DataType>
auto df::TResource<DataType>::GetContainer() const -> const DataContainer* {
	return m_Data;
}

template <typename DataType>
auto df::TResource<DataType>::GetData()->DataType& {
	DFAssert(m_Data != nullptr, "trying to get uninitialized resource data!");

	return m_Data->GetData();
}

template <typename DataType>
auto df::TResource<DataType>::GetData()const-> const DataType& {
	DFAssert(m_Data != nullptr, "trying to get uninitialized resource data!");

	return m_Data->GetData();
}

template <typename DataType>
auto df::TResource<DataType>::operator -> ()->DataType* {
	DFAssert(m_Data != nullptr, "trying to get uninitialized resource data!");

	return &m_Data->GetData();
}

template <typename DataType>
auto df::TResource<DataType>::operator -> () const -> const DataType* {
	DFAssert(m_Data != nullptr, "trying to get uninitialized resource data!");

	return &m_Data->GetData();
}

template <typename DataType>
df::TResource<DataType>::operator bool() const {
	return m_Data != nullptr;
}


template<typename ResourceType>
int df::ResourceInfo<ResourceType>::GetResourceTypeId() {
	return s_ResourceTypeId;
}

template<typename ResourceType>
int df::ResourceInfo<ResourceType>::GetResourceTypeIdStaticTime() {
	static int id = df::ResourceTypeRegistry::s_TypeCount++;
	static_assert(std::is_base_of<TResource<ResourceType::Data>, ResourceType>::value, "ResourceType should be inherited from TResource!");
	return id;
}