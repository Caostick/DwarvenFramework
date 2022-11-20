#pragma once

#include <DwarfConstructorRender/Types.h>

#include <DwarvenCore/Types.h>

namespace rf {
	
	enum class EShaderStage {
		Vertex = 1 << 0,
		TesselationControl = 1 << 1,
		TesselationEvaluation = 1 << 2,
		Geometry = 1 << 3,
		Fragment = 1 << 4,
		Compute = 1 << 5,
		Raygen = 1 << 6,
		AnyHit = 1 << 7,
		ClosestHit = 1 << 8,
		Miss = 1 << 9,
		Intersection = 1 << 10
	};

	using ShaderStageFlags = df::EnumFlags<EShaderStage>;
	DF_DEFINE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

	struct DescriptorLayout {
		DescriptorLayout() = default;
		DescriptorLayout(DescriptorLayout&&) = default;
		DescriptorLayout(const DescriptorLayout&) = default;
		DescriptorLayout& operator = (DescriptorLayout&&) = default;
		DescriptorLayout& operator = (const DescriptorLayout&) = default;

		rf::EDescriptorType m_Descriptors[16] = {};
	};

	struct DescriptorDefinition {
		DescriptorDefinition() = default;
		DescriptorDefinition(DescriptorDefinition&&) = default;
		DescriptorDefinition(const DescriptorDefinition&) = default;
		DescriptorDefinition& operator = (DescriptorDefinition&&) = default;
		DescriptorDefinition& operator = (const DescriptorDefinition&) = default;

		uint32 m_BindingIndex = 0;
		EDescriptorType m_Type = EDescriptorType::SamplerState;
		ShaderStageFlags m_ShaderStageFlags;
		uint32 m_Count = 1;
	};
}