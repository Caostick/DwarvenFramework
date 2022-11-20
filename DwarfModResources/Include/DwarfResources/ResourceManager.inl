#pragma once

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/StringOperations.h>

#include <iostream>

template<typename T>
df_private::ResourceTypeContainer<T>::ResourceTypeContainer(const df::String& typeName) 
	: IResourceTypeContainer(typeName) {
}

template<typename T>
df_private::ResourceTypeContainer<T>::~ResourceTypeContainer() {
	for (auto* loader : m_Loaders) {
		DFDelete loader;
	}
}

template<typename T>
auto df_private::ResourceTypeContainer<T>::GetLoaders() const -> const df::Vector<df::IResourceLoader*>& {
	return m_Loaders;
}

template<typename T>
auto df_private::ResourceTypeContainer<T>::GetResourceIndex(const df::StringView& name) const -> int {
	for (int i = 0; i < m_Resources.size(); ++i) {
		if (m_Resources[i].m_Name == name) {
			return i;
		}
	}

	return -1;
}

template<typename T>
auto df_private::ResourceTypeContainer<T>::GetResourceContainer(int index)->df_private::ResourceContainer& {
	return m_Resources[index];
}

template<typename T>
void df_private::ResourceTypeContainer<T>::UnloadResources() {
	for (int i = 0; i < static_cast<int>(m_Resources.size()); ++i) {
		if (!m_Resources[i].m_Resource) {
			continue;
		}

		m_Loaders[m_Resources[i].m_LoaderIndex]->UnloadResource(m_Resources[i].m_Resource);
	}
}

template<typename T>
void df_private::ResourceTypeContainer<T>::ResetResources() {
	for (int i = 0; i < m_Resources.size(); ++i) {
		DFDelete m_Resources[i].m_Resource;
	}
	m_Resources.clear();
}

template<typename T>
void df_private::ResourceTypeContainer<T>::PushResource(const df::StringView& name, const df::StringView& extension, int moduleIndex) {
	for (int i = 0; i < m_Loaders.size(); ++i) {
		const bool fitsExtension = df::CompareNoCase<char>(m_Loaders[i]->GetResourceExtension(), extension);
		if (!fitsExtension) {
			continue;
		}

		int resourceIndex = GetResourceIndex(name);
		if (resourceIndex == -1) {
			df_private::ResourceContainer newContainer;
			newContainer.m_Name = df::String(name);
			m_Resources.emplace_back(newContainer);

			resourceIndex = int(m_Resources.size()) - 1;
		}

		df_private::ResourceContainer& resourceContainer = GetResourceContainer(resourceIndex);

		const bool greaterModulePriority = (moduleIndex > resourceContainer.m_ModuleIndex);
		const bool equalModulePriority = (moduleIndex == resourceContainer.m_ModuleIndex);
		const bool greaterLoaderPriority = (i > resourceContainer.m_LoaderIndex);

		if (greaterModulePriority || equalModulePriority && greaterLoaderPriority) {
			resourceContainer.m_ModuleIndex = moduleIndex;
			resourceContainer.m_LoaderIndex = i;
		}
	}
}

template<typename T>
template<typename LT, typename... TArgs>
void df_private::ResourceTypeContainer<T>::RegisterLoader(TArgs&&... args) {
	m_Loaders.emplace_back(DFNew LT(std::forward<TArgs>(args)...));
}









template<typename T, typename LT, typename... TArgs>
void df::ResourceManager::RegisterResourceLoader(TArgs&&... args) {
	const int resTypeId = df::ResourceInfo<T>::GetResourceTypeId();

	m_ResourceTypeContainers.resize(std::max(int(m_ResourceTypeContainers.size()), resTypeId + 1));

	if (!m_ResourceTypeContainers[resTypeId]) {
		df::String typeName;
		if (!strncmp("struct", typeid(T).name(), 6)) {
			typeName = typeid(T).name() + 7;
		} else if (!strncmp("class", typeid(T).name(), 5)) {
			typeName = typeid(T).name() + 6;
		} else {
			typeName = "Unknown";
		}

		m_ResourceTypeContainers[resTypeId] = DFNew df_private::ResourceTypeContainer<T>(typeName);
	}

	auto* resTypeContainer = reinterpret_cast<df_private::ResourceTypeContainer<T>*>(m_ResourceTypeContainers[resTypeId]);

	resTypeContainer->RegisterLoader<LT>(std::forward<TArgs>(args)...);
}

template<typename T>
void df::ResourceManager::MapResourcesOfType() {
	const int resTypeId = df::ResourceInfo<T>::GetResourceTypeId();
	MapResourcesByResourceId(resTypeId);
}

/*
template<typename T>
bool df::ResourceManager::IsLoaded(const df::ResourceId& resourceId) const {
	DFAssert(resourceId.m_IsAssigned, "Resource id wasn't assigned!");
	DFAssert(resourceId.m_IsValid, "Resource id is not valid!");
	DFAssert(resourceId.m_ResourceTypeId == df::ResourceInfo<T>::GetResourceTypeId(), "Resource type is not valid!");

	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	const auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);

	return resourceContainer.m_Status == df_private::EResourceStatus::Ready;
}
*/

/*
template<typename T>
bool df::ResourceManager::RequestLoad(const df::ResourceId& resourceId) {
	DFAssert(resourceId.m_IsAssigned, "Resource id wasn't assigned!");
	DFAssert(resourceId.m_IsValid, "Resource id is not valid!");
	DFAssert(resourceId.m_ResourceTypeId == df::ResourceInfo<T>::GetResourceTypeId(), "Resource type is not valid!");

	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);

	DFAssert(resourceContainer.m_Resource, "Resource wasn't mapped!");

	const auto& moduleName = m_ActiveModules[resourceContainer.m_ModuleIndex];
	const auto* loader = resourceTypeContainer->GetLoaders()[resourceContainer.m_LoaderIndex];
	const auto filename = MakeFilePath(resourceContainer.m_ModuleIndex, resourceContainer.m_Name, loader->GetResourceExtension());
	const auto file = m_FileSystem.OpenFile(filename, df::EFileAccess::Read);
	if (file) {
		const auto fileData = file->MapData();
		resourceContainer.m_Resource = loader->LoadResource(fileData.GetData(), fileData.GetSize());

		m_FileSystem.CloseFile(file);
	}

	resourceContainer.m_Status = df_private::EResourceStatus::Ready;
}
*/

/*
template<typename T>
auto df::ResourceManager::GetResource(const df::ResourceId& resourceId) const -> const T* {
	DFAssert(resourceId.m_IsAssigned, "Resource id wasn't assigned!");
	DFAssert(resourceId.m_IsValid, "Resource id is not valid!");
	DFAssert(resourceId.m_ResourceTypeId == df::ResourceInfo<T>::GetResourceTypeId(), "Resource type is not valid!");

	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);

	DFAssert(resourceContainer.m_Status == df_private::EResourceStatus::Ready, "Trying to get resource, what is not ready!");

	return reinterpret_cast<T*>(resourceContainer.m_Resource);
}
*/

/*
template<typename T>
auto df::ResourceManager::GetResource(const df::StringView& name) const -> const T* {
	const df::ResourceId resourceId = GetResourceId<T>(name);

	DFAssert(resourceId.m_IsAssigned, "Resource id wasn't assigned!");
	DFAssert(resourceId.m_IsValid, "Resource id is not valid!");

	return GetResource<T>(resourceId);
}
*/

template<typename T>
auto df::ResourceManager::GetResource(const df::StringView& name) const -> df::Resource<T> {
	const df::ResourceId resourceId = GetResourceId<T>(name);

	if (!resourceId.m_IsValid) {
		return df::Resource<T>(this, ResourceId::Unassigned, nullptr);
	}

	DFAssert(resourceId.m_IsAssigned, "Resource id wasn't assigned!");
	DFAssert(resourceId.m_IsValid, "Resource id is not valid!");
	DFAssert(resourceId.m_ResourceTypeId == df::ResourceInfo<T>::GetResourceTypeId(), "Resource type is not valid!");

	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);

	const T* resource = reinterpret_cast<T*>(resourceContainer.m_Resource);

	return df::Resource<T>(this, resourceId, resource);
}

template<typename T>
void df::ResourceManager::AcquireResource([[maybe_unused]] const df::ResourceId& resourceId) const {
	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);
	resourceContainer.m_RefCount++;

	//std::cout << "[" << resourceId.m_ResourceTypeId << "." << resourceId.m_ResourceIndex << "](" << resourceContainer.m_RefCount << ")\n";
}

template<typename T>
void df::ResourceManager::FreeResource([[maybe_unused]] const df::ResourceId& resourceId) const {
	auto* resourceTypeContainer = m_ResourceTypeContainers[resourceId.m_ResourceTypeId];
	auto& resourceContainer = resourceTypeContainer->GetResourceContainer(resourceId.m_ResourceIndex);
	resourceContainer.m_RefCount--;

	//std::cout << "[" << resourceId.m_ResourceTypeId << "." << resourceId.m_ResourceIndex << "](" << resourceContainer.m_RefCount << ")\n";
}

template<typename T>
auto df::ResourceManager::GetResourceId(const df::StringView& name) const->df::ResourceId {
	const int resTypeId = df::ResourceInfo<T>::GetResourceTypeId();

	if (m_ActiveModuleSetIndex <= 0) {
		return df::ResourceId::Invalid;
	}

	if (resTypeId >= m_ResourceTypeContainers.size()) {
		return df::ResourceId::Invalid;
	}

	auto* resourceTypeContainer = m_ResourceTypeContainers[resTypeId];
	if (!resourceTypeContainer) {
		return df::ResourceId::Invalid;
	}

	const int resourceIndex = resourceTypeContainer->GetResourceIndex(name);
	if (resourceIndex == -1) {
		return df::ResourceId::Invalid;
	}

	return df::ResourceId({ uint64(1), uint64(1), uint64(m_ActiveModuleSetIndex), uint64(resTypeId), uint64(resourceIndex) });
}