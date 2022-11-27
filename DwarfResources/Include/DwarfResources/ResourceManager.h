#pragma once

#include <DwarfResources/Resource.h>
#include <DwarfResources/ResourceLoader.h>

#include <DwarfFileSystem/FileSystem.h>
#include <DwarfFileSystem/File.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/HashMap.h>

namespace df {
	class IResource;
	class IResourceLoader;
	class Data;

	template <typename T>
	struct ResourceSignals;
}

namespace df_private {
	struct ActiveModuleSet {
		ActiveModuleSet() = default;
		ActiveModuleSet(const ActiveModuleSet&) = default;
		ActiveModuleSet(ActiveModuleSet&&) = default;
		ActiveModuleSet& operator = (const ActiveModuleSet&) = default;
		ActiveModuleSet& operator = (ActiveModuleSet&&) = default;

		df::Vector<df::String> m_ActiveModules;
	};

	struct ResourceContainer {
		df::IResource* m_Resource = nullptr;

		df::String m_Name;
		int m_ModuleIndex = -1;
		int m_LoaderIndex = -1;
		int m_RefCount = 0;
	};

	class IResourceTypeContainer {
	public:
		IResourceTypeContainer(const df::String& typeName) : m_TypeName(typeName) {}
		virtual ~IResourceTypeContainer() {}

		virtual auto GetLoaders() const -> const df::Vector<df::IResourceLoader*> & = 0;
		virtual auto GetResourceIndex(const df::StringView& name) const -> int = 0;
		virtual auto GetResourceContainer(int index)->df_private::ResourceContainer& = 0;
		virtual void UnloadResources() = 0;
		virtual void ResetResources() = 0;
		virtual void PushResource(const df::StringView& name, const df::StringView& extension, int moduleIndex) = 0;

		auto GetTypeName() const -> const df::String& { return m_TypeName; }
		auto GetLoaders()->const df::Vector<df::IResourceLoader*>& { return m_Loaders; }
		auto GetResources()->df::Vector<df_private::ResourceContainer>& { return m_Resources; }

	protected:
		df::Vector<df::IResourceLoader*> m_Loaders;
		df::Vector<df_private::ResourceContainer> m_Resources;
		df::String m_TypeName;
	};

	template<typename T>
	class ResourceTypeContainer : public df_private::IResourceTypeContainer {
	public:
		ResourceTypeContainer(const df::String& typeName);
		~ResourceTypeContainer() override;

		auto GetLoaders() const -> const df::Vector<df::IResourceLoader*> & override;
		auto GetResourceIndex(const df::StringView& name) const -> int override;
		auto GetResourceContainer(int index)->df_private::ResourceContainer& override;
		void UnloadResources() override;
		void ResetResources() override;
		void PushResource(const df::StringView& name, const df::StringView& extension, int moduleIndex) override;

		template<typename LT, typename... TArgs>
		void RegisterLoader(TArgs&&... args);
	};
}

namespace df {
	class ResourceManager {
		template<typename T>
		friend class df::Resource;
	public:
		ResourceManager(const df::FileSystem& fileSystem, const df::StringView& dataPath);
		~ResourceManager();

		void SetupModules(const df::Vector<df::String>& activeModules);
		void ParseData();
		void UnloadResources();
		void ResetResources();
		void PushResource(const df::StringView& name, const df::StringView& module, const df::StringView& extension);
		void Update();

		void MapResources();

		template<typename T, typename LT, typename... TArgs>
		void RegisterResourceLoader(TArgs&&... args);

		template<typename T>
		void MapResourcesOfType();

		template<typename T>
		auto GetResource(const df::StringView& name) const -> df::Resource<T>;

	private:
		template<typename T>
		void AcquireResource(const df::ResourceId& resourceId) const;

		template<typename T>
		void FreeResource(const df::ResourceId& resourceId) const;

		template<typename T>
		auto GetResourceId(const df::StringView& name) const->df::ResourceId;

		void MapResourcesByResourceId(int resTypeId);

		auto MakeFilePath(int moduleIndex, const df::StringView& name, const df::StringView& extension)->df::String;

		const df::FileSystem& m_FileSystem;
		const df::String m_DataPath;

		df::Vector<df_private::ActiveModuleSet> m_ModuleSets;
		int m_ActiveModuleSetIndex;

		//df_private::IResourceTypeContainer* m_ResourceTypeContainers[df::MaxResourceTypeCount] = {nullptr};
		df::Vector<df_private::IResourceTypeContainer*> m_ResourceTypeContainers;
	};
}

#include <DwarfResources/ResourceManager.inl>