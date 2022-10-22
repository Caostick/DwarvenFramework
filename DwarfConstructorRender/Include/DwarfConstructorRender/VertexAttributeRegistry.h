#pragma once

#include <DwarfConstructorRender/Types.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/StringView.h>

namespace rf {
	constexpr int MaxVertexAttributeTypes = 128;
	struct VertexAttributeRegistry {
		struct Entry {
			const char* Name = nullptr;
			rf::EVertexAttributeFormat Format = EVertexAttributeFormat::X32_Float;
			uint32 SizeOf = 0;
		};

		static uint32 TypeCount;
		static Entry Entries[MaxVertexAttributeTypes];

		static auto FindByName(const df::StringView& name) -> int32;
	};

	struct VertexAttributeLocation {
		uint32 m_AttributeIndex;
		uint32 m_ShaderLocation;
	};

	template<typename T>
	struct VertexAttributeTypeInfo {
		static uint32 Id;
		static auto GetId()->uint32;
	private:
		static auto Create()->uint32;
	};

	auto ToShaderInOutType(rf::EVertexAttributeFormat format)->rf::EShaderInOutType;
	auto ToAttributeSize(rf::EVertexAttributeFormat format)->uint32;
	auto ToAttributeSize(rf::EShaderInOutType type) -> uint32;
	auto ToShaderInOutString(rf::EShaderInOutType type) -> const char*;
}

template<typename T>
uint32 rf::VertexAttributeTypeInfo<T>::Id = rf::VertexAttributeTypeInfo<T>::GetId();

#include <DwarfConstructorRender/VertexAttributeRegistry.inl>