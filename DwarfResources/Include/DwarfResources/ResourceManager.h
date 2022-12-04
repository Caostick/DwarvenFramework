#pragma once

#include <DwarfResources/Resource.h>
#include <DwarfResources/ResourceLoader.h>
#include <DwarfResources/ResourceTypeManager.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/ObjectPool.h>

#include <functional>

namespace df {
	class FileSystem;
}

namespace df {
	class ResourceManager {
	public:
		ResourceManager(const FileSystem& fileSystem, const StringView& dataPath);
		~ResourceManager();

		bool IndexData();
		void Clear();

		void Update();

		template<typename ResourceType>
		bool RequestResource(const StringView& guid, const std::function<void(const ResourceType&)>& callback);

		template<typename ResourceType, typename T>
		bool RequestResource(const StringView& guid, T* inst, void (T::* func)(const ResourceType&));

		template<typename ResourceType, typename LoaderType, typename... TArgs>
		void RegisterResourceLoader(TArgs&&... args);

	private:
		const FileSystem& m_FileSystem;
		const String m_DataPath;

		Vector<ResourceTypeManager*> m_ResourceTypeManagers;
	};
}

#include <DwarfResources/ResourceManager.inl>