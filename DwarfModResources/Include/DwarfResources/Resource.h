#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace df {
	class ResourceManager;
}

namespace df {
	struct ResourceId {
		ResourceId() = default;
		ResourceId(const ResourceId&) = default;
		ResourceId(ResourceId&&) = default;
		ResourceId& operator = (const ResourceId&) = default;
		ResourceId& operator = (ResourceId&&) = default;

		auto operator == (const ResourceId& other) const -> bool;
		auto operator != (const ResourceId& other) const -> bool;

		// Tells if resource is mapped
		uint64 m_IsValid : 1;

		// Tells if id was assigned
		uint64 m_IsAssigned : 1;

		// Module set id in resource manager. Needed to prevent of using resourceId after changing module set.
		uint64 m_ModuleSet : 8;

		// Resource type id. Corresponds to resource type container in resource manager
		uint64 m_ResourceTypeId : 10;

		// Resource index in resource type container
		uint64 m_ResourceIndex : 44;

		static const ResourceId Unassigned;
		static const ResourceId Invalid;
	};

	template<typename T>
	class Resource {
	public:
		Resource();
		Resource(const df::ResourceManager* resourceManager, const df::ResourceId resourceId, const T* resource);
		Resource(const Resource& other);
		Resource(Resource&& other);
		~Resource();

		void Clear();
		bool IsSet();

		auto operator = (const Resource& other) -> df::Resource<T>&;
		auto operator = (Resource&& other) -> df::Resource<T>&;

		operator bool() const { return m_Resource != nullptr; }
		auto operator->() const -> const T*;

	private:
		df::ResourceId m_ResourceId = df::ResourceId::Unassigned;

		const df::ResourceManager* m_ResourceManager = nullptr;
		const T* m_Resource = nullptr;
	};

	struct ResourceCapacity {
		static int s_ResourceTypeCount;
	};

	template<typename T>
	struct ResourceInfo {
	public:
		static int s_ResourceTypeId;

		static int GetResourceTypeId();

	private:
		static int GetResourceTypeIdStaticTime();
	};

	class IResource {
	public:
		virtual ~IResource() {}
	};

	template<typename T>
	int df::ResourceInfo<T>::s_ResourceTypeId = df::ResourceInfo<T>::GetResourceTypeIdStaticTime();
}

#include <DwarfResources/Resource.inl>