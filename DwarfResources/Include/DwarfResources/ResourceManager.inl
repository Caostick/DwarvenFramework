#pragma once

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/StringOperations.h>

#include <iostream>

template<typename ResourceType>
bool df::ResourceManager::RequestResource(const StringView& guid, const std::function<void(const ResourceType&)>& callback) {
	const int resTypeId = ResourceInfo<ResourceType>::GetResourceTypeId();

	auto* resTypeManager = reinterpret_cast<TResourceTypeManager<ResourceType>*>(m_ResourceTypeManagers[resTypeId]);

	return resTypeManager->RequestResource(guid, callback);
}

template<typename ResourceType, typename T>
bool df::ResourceManager::RequestResource(const StringView& guid, T* inst, void (T::* func)(const ResourceType&)) {
	return RequestResource(guid, std::function([=](const ResourceType& res) {	(inst->*func)(res);	} ));
}

template<typename ResourceType, typename LoaderType, typename... TArgs>
void df::ResourceManager::RegisterResourceLoader(TArgs&&... args) {
	const int resTypeId = ResourceInfo<ResourceType>::GetResourceTypeId();

	m_ResourceTypeManagers.resize(std::max(int(m_ResourceTypeManagers.size()), resTypeId + 1));

	if (!m_ResourceTypeManagers[resTypeId]) {
		String typeName;
		if (!strncmp("struct", typeid(ResourceType).name(), 6)) {
			typeName = typeid(ResourceType).name() + 7;
		} else if (!strncmp("class", typeid(ResourceType).name(), 5)) {
			typeName = typeid(ResourceType).name() + 6;
		} else {
			typeName = "Unknown";
		}

		m_ResourceTypeManagers[resTypeId] = DFNew TResourceTypeManager<ResourceType>(m_FileSystem, m_DataPath);
	}

	auto* resTypeManager = reinterpret_cast<TResourceTypeManager<ResourceType>*>(m_ResourceTypeManagers[resTypeId]);

	resTypeManager->RegisterResourceLoader<LoaderType>(std::forward<TArgs>(args)...);
}