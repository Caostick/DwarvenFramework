#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace df {
	template<typename DataType>
	class TResource {
	public:
		using Data = DataType;
		struct DataContainer {
			DataContainer(const StringView& guid, const StringView& path, const StringView& ext);

			auto GetRefCount() const->int32;
			auto GetGUID() const->const String&;
			auto GetPath() const->const String&;
			auto GetExt() const->const String&;
			auto GetData()->Data&;
			auto GetData() const -> const Data&;

			auto TakeOne()->int32;
			auto FreeOne()->int32;

		private:
			int32 m_RefCount = 0;
			String m_GUID;
			String m_Path;
			String m_Ext;
			Data m_Data;
		};

	public:
		TResource();
		TResource(DataContainer* data);
		TResource(const TResource& other);
		TResource(TResource&& other);
		~TResource();

		auto operator = (const TResource& other) -> TResource&;
		auto operator = (TResource&& other) -> TResource&;

		auto GetContainer() -> DataContainer*;
		auto GetContainer() const -> const DataContainer*;

		auto GetData()->DataType&;
		auto GetData()const-> const DataType&;

		auto operator -> () -> DataType*;
		auto operator -> () const -> const DataType*;

		operator bool() const;

	private:
		DataContainer* m_Data = nullptr;
	};

	struct ResourceTypeRegistry {
		static int s_TypeCount;
	};

	template<typename ResourceType>
	struct ResourceInfo {
	public:
		static int s_ResourceTypeId;

		static int GetResourceTypeId();

	private:
		static int GetResourceTypeIdStaticTime();
	};

	template<typename ResourceType>
	int ResourceInfo<ResourceType>::s_ResourceTypeId = ResourceInfo<ResourceType>::GetResourceTypeIdStaticTime();
}

#include <DwarfResources/Resource.inl>