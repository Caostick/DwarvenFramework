#pragma once

#include <DwarfResources/ResourceTypeManager.h>

#include <DwarfFileSystem/FileSystem.h>
#include <DwarfFileSystem/FileData.h>
#include <DwarfFileSystem/File.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/DataVisitor.h>

template<typename ResourceType>
df::TResourceTypeManager<ResourceType>::TResourceTypeManager(const FileSystem& fileSystem, const StringView& dataPath) 
	: m_FileSystem(fileSystem)
	, m_DataPath(dataPath) {
}

template<typename ResourceType>
df::TResourceTypeManager<ResourceType>::~TResourceTypeManager() {
	for(auto loader : m_Loaders) {
		DFDelete loader;
	}
}

template<typename ResourceType>
template<typename LoaderType, typename... TArgs>
void df::TResourceTypeManager<ResourceType>::RegisterResourceLoader(TArgs&&... args) {
	m_Loaders.emplace_back(DFNew LoaderType(std::forward<TArgs>(args)...));
}

template<typename ResourceType>
bool df::TResourceTypeManager<ResourceType>::PushResource(const StringView& guid, const StringView& path, const StringView& ext) {
	for (auto&& loader : m_Loaders) {
		if (CompareNoCase<char>(loader->GetResourceExtension(), ext)) {
			m_Resources.Create(guid, path, ext);
			return true;
		}
	}

	return false;
}

template<typename ResourceType>
bool df::TResourceTypeManager<ResourceType>::RequestResource(const StringView& guid, const std::function<void(const ResourceType&)>& callback) {
	for (auto&& request : m_RequestedResources) {
		auto&& resource = request->m_Resource;
		auto container = resource.GetContainer();
		if (CompareNoCase<char>(container->GetGUID(), guid)) {
			request->m_Callbacks.push_back(callback);
			return true;
		}
	}

	for (auto container : m_Resources) {
		if (CompareNoCase<char>(container->GetGUID(), guid)) {
			auto request = DFNew ResourceRequest<ResourceType>;
			request->m_Resource = ResourceType(container);
			request->m_Callbacks.push_back(callback);
			m_RequestedResources.push_back(request);
			return true;
		}
	}

	return false;
}

template<typename ResourceType>
void df::TResourceTypeManager<ResourceType>::Clear() {
	m_Resources.Clear();
}

template<typename ResourceType>
void df::TResourceTypeManager<ResourceType>::Update() {
	for (auto&& request : m_RequestedResources) {
		ResourceType& resource = request->m_Resource;
		typename ResourceType::DataContainer* container = resource.GetContainer();
		const String& ext = container->GetExt();

		for (auto&& loader : m_Loaders) {
			if (CompareNoCase<char>(loader->GetResourceExtension(), ext)) {
				auto file = m_FileSystem.OpenFile(container->GetPath(), EFileAccess::Read);
				auto data = file->MapData();
				loader->Load(container->GetData(), container->GetPath(), container->GetGUID(), data.GetData(), data.GetSize());
				m_FileSystem.CloseFile(file);
			}
		}

		for (auto&& callback : request->m_Callbacks) {
			callback(request->m_Resource);
		}

		DFDelete request;
	}
	m_RequestedResources.clear();
}