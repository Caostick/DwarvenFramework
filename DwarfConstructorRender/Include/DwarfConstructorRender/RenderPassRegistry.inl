#pragma once

#include <DwarvenCore/TypeList.h>

namespace {
	template<typename TL, typename Enable = void>
	struct RenderPassTargetConstructor;

	template<typename TL>
	struct RenderPassTargetConstructor<TL, typename std::enable_if<df::Length<TL>::value == 0>::type> {
		static void Construct(uint32, uint32*) {}
	};

	template<typename TL>
	struct RenderPassTargetConstructor<TL, typename std::enable_if<df::Length<TL>::value != 0>::type> {
	public:
		static void Construct(uint32 idx, uint32* targets) {
			using RenderTargetType = typename TL::Head;
			static_assert(std::is_base_of<rf::RenderTarget, RenderTargetType>::value,
				"Render pass should contain only RenderTarget derived classes!"
				);

			const uint32 rtId = rf::RenderTargetTypeInfo<RenderTargetType>::GetId();
			targets[idx] = rtId;

			RenderPassTargetConstructor<typename TL::Tail>::Construct(idx + 1, targets);
		}
	};
}

template<typename T>
auto rf::RenderPassTypeInfo<T>::GetId()->uint32 {
	static uint32 id = Create();
	return id;
}

template<typename T>
auto rf::RenderPassTypeInfo<T>::Create()->uint32 {
	static_assert(std::is_base_of<rf::RenderPass, T>::value);
	const uint32 id = rf::RenderPassRegistry::TypeCount++;

	auto& entry = rf::RenderPassRegistry::Entries[id];

	entry.m_Name = T::Name;
	if constexpr (!df::IsEmpty<typename T::Inputs>::value) {
		entry.m_InputRenderTargetCount = df::Length<typename T::Inputs>::value;
		RenderPassTargetConstructor<typename T::Inputs>::Construct(0, entry.m_InputRenderTargets);
	}

	if constexpr (!df::IsEmpty<typename T::Outputs>::value) {
		entry.m_OutputRenderTargetCount = df::Length<typename T::Outputs>::value;
		RenderPassTargetConstructor<typename T::Outputs>::Construct(0, entry.m_OutputRenderTargets);
	}

	//rf::RenderPassRegistry::ParamSetDefinitions[id];
	//rf::RenderPassRegistry::RenderPasses[id];
	//rf::RenderPassRegistry::DescriptorSetLayouts[id];
	//rf::RenderPassRegistry::PipilineGenerators[id];

	return id;
}