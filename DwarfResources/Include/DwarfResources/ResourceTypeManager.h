#pragma once

#include <DwarfResources/ResourceLoader.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/StringView.h>

#include <functional>

namespace df {
	class FileSystem;
}

namespace df {
	template<typename ResourceType>
	struct ResourceRequest {
		ResourceType m_Resource;
		Vector<std::function<void(const ResourceType&)>> m_Callbacks;
	};

    class ResourceTypeManager {
	public:
		virtual ~ResourceTypeManager() {}

		virtual bool PushResource(const StringView& guid, const StringView& path, const StringView& ext) = 0;

		virtual void Clear() = 0;

		virtual void Update() = 0;
	};

	template<typename ResourceType>
	class TResourceTypeManager : public ResourceTypeManager {
	public:
		TResourceTypeManager(const FileSystem& fileSystem, const StringView& dataPath);
		~TResourceTypeManager() override;

		template<typename LoaderType, typename... TArgs>
		void RegisterResourceLoader(TArgs&&... args);

		bool PushResource(const StringView& guid, const StringView& path, const StringView& ext) override;

		bool RequestResource(const StringView& guid, const std::function<void(const ResourceType&)>& callback);

		void Clear() override;

		void Update() override;

	private:
		const FileSystem& m_FileSystem;
		const String m_DataPath;

		Vector<TResourceLoader<ResourceType>*> m_Loaders;

		ObjectPool<typename ResourceType::DataContainer> m_Resources;

		Vector<ResourceRequest<ResourceType>*> m_RequestedResources;
	};
}

#include <DwarfResources/ResourceTypeManager.inl>