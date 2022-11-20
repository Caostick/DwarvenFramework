#pragma once

#include <DwarfResources/ResourceManager.h>

template<typename T>
df::Resource<T>::Resource() 
	: m_ResourceId(df::ResourceId::Unassigned)
	, m_ResourceManager(nullptr)
	, m_Resource(nullptr) {
}

template<typename T>
df::Resource<T>::Resource(const df::ResourceManager* resourceManager, const df::ResourceId resourceId, const T* resource)
	: m_ResourceManager(resourceManager)
	, m_ResourceId(resourceId)
	, m_Resource(resource) {

	if (m_Resource) {
		m_ResourceManager->AcquireResource<T>(resourceId);
	}
}

template<typename T>
df::Resource<T>::Resource(const Resource& other) {
	m_ResourceId = other.m_ResourceId;
	m_ResourceManager = other.m_ResourceManager;
	m_Resource = other.m_Resource;

	if (m_Resource) {
		m_ResourceManager->AcquireResource(m_ResourceId);
	}
}

template<typename T>
df::Resource<T>::Resource(Resource&& other) {
	m_ResourceId = other.m_ResourceId;
	m_ResourceManager = other.m_ResourceManager;
	m_Resource = other.m_Resource;

	other.m_ResourceManager = nullptr;
	other.m_Resource = nullptr;
	other.m_ResourceId = df::ResourceId::Unassigned;
}

template<typename T>
df::Resource<T>::~Resource() {
	if (m_Resource) {
		Clear();
	}
}

template<typename T>
void df::Resource<T>::Clear() {
	if (m_ResourceManager) {
		m_ResourceManager->FreeResource<T>(m_ResourceId);
	}

	m_ResourceManager = nullptr;
	m_Resource = nullptr;
	m_ResourceId = df::ResourceId::Unassigned;
}

template<typename T>
bool df::Resource<T>::IsSet() {
	return m_ResourceId != df::ResourceId::Unassigned;
}

template<typename T>
auto df::Resource<T>::operator = (const df::Resource<T>& other) -> df::Resource<T>& {
	m_ResourceId = other.m_ResourceId;
	m_ResourceManager = other.m_ResourceManager;
	m_Resource = other.m_Resource;

	m_ResourceManager->AcquireResource<T>(m_ResourceId);

	return *this;
}

template<typename T>
auto df::Resource<T>::operator = (df::Resource<T>&& other) -> df::Resource<T>& {
	m_ResourceId = other.m_ResourceId;
	m_ResourceManager = other.m_ResourceManager;
	m_Resource = other.m_Resource;

	other.m_ResourceManager = nullptr;
	other.m_Resource = nullptr;
	other.m_ResourceId = df::ResourceId::Unassigned;

	return *this;
}

template<typename T>
auto df::Resource<T>::operator->() const -> const T* {
	return m_Resource;
}

template<typename T>
int df::ResourceInfo<T>::GetResourceTypeId() {
	return s_ResourceTypeId;
}

template<typename T>
int df::ResourceInfo<T>::GetResourceTypeIdStaticTime() {
	static int id = df::ResourceCapacity::s_ResourceTypeCount++;
	static_assert(std::is_base_of<df::IResource, T>::value, "Type should be inherited from Resource!");
	return id;
}