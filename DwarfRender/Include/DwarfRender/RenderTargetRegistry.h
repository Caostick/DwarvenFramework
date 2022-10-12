#pragma once

#include <DwarfRender/Types.h>
#include <DwarfRender/ObjectId.h>

#include <DwarvenCore/String.h>

namespace rf {
	constexpr int MaxRenderTargetTypes = 128;
	struct RenderTargetRegistry {
		struct Entry {
			const char* m_Name = nullptr;
			rf::ETextureFormat m_Format = ETextureFormat::Undefined;
		};

		static uint32 TypeCount;
		static Entry Entries[MaxRenderTargetTypes];
	};

	template<typename T>
	struct RenderTargetTypeInfo {
		static uint32 Id;
		static auto GetId() -> uint32;
	private:
		static auto Create()->uint32;
	};
}

template<typename T>
uint32 rf::RenderTargetTypeInfo<T>::Id = rf::RenderTargetTypeInfo<T>::GetId();

#include <DwarfRender/RenderTargetRegistry.inl>