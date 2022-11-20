#pragma once

#include <DwarvenCore/Types.h>

namespace rf {
	struct RenderViewCommon {
		static uint32 TypeCount;
	};

	template<typename T>
	struct RenderViewTypeInfo {
		static uint32 Id;
		static auto GetId()->uint32;
	private:
		static auto Create()->uint32;
	};
}

template<typename T>
uint32 rf::RenderViewTypeInfo<T>::Id = rf::RenderViewTypeInfo<T>::GetId();

#include <DwarfConstructorRender/RenderViewRegistry.inl>