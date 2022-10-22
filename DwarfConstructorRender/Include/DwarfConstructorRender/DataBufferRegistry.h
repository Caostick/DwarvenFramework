#pragma once

#include <DwarvenCore/Types.h>

namespace rf {
	constexpr int MaxDataBufferTypes = 128;
	struct DataBufferRegistry {
		struct Entry {
			const char* m_Name = nullptr;
		};

		static uint32 TypeCount;
		static Entry Entries[MaxDataBufferTypes];
	};

	template<typename T>
	struct DataBufferTypeInfo {
		static uint32 Id;
		static auto GetId()->uint32;
	private:
		static auto Create()->uint32;
	};
}

template<typename T>
uint32 rf::DataBufferTypeInfo<T>::Id = rf::DataBufferTypeInfo<T>::GetId();

#include <DwarfConstructorRender/DataBufferRegistry.inl>