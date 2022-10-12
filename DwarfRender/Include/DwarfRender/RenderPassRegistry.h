#pragma once

#include <DwarfRender/ObjectId.h>
#include <DwarfRender/RenderTarget.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/TypeList.h>

namespace rf {
	class ParamSetDefinition;
	class ParamSetDeclarator;

	namespace sg {
		class GraphicsPipelineGenerator;
	}
}

namespace rf {
	constexpr int MaxRenderPassTypes = 128;
	constexpr int MaxRenderPassTargets = 16;
	struct RenderPassRegistry {
		struct Entry {
			const char* m_Name = nullptr;
			rf::ParamSetDefinition* m_ParamSetDefinition = nullptr;
			uint32 m_InputRenderTargetCount = 0;
			uint32 m_OutputRenderTargetCount = 0;
			uint32 m_InputRenderTargets[MaxRenderPassTargets] = { 0 };
			uint32 m_OutputRenderTargets[MaxRenderPassTargets] = { 0 };
			rf::PassId m_RenderPass = nullptr;
			sg::GraphicsPipelineGenerator* m_PipilineGenerator = nullptr;
		};

		static uint32 TypeCount;
		static Entry Entries[MaxRenderPassTypes];
	};

	template<typename T>
	struct RenderPassTypeInfo {
		static uint32 Id;
		static auto GetId()->uint32;
	private:
		static auto Create()->uint32;
	};
}

template<typename T>
uint32 rf::RenderPassTypeInfo<T>::Id = rf::RenderPassTypeInfo<T>::GetId();

#include <DwarfRender/RenderPassRegistry.inl>