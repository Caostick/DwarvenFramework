#pragma once

#include "VkAPI.h"

namespace vk {
	struct SamplerState {
		SamplerState() = default;
		SamplerState(const SamplerState&) = default;
		SamplerState(SamplerState&&) = default;
		SamplerState& operator = (const SamplerState&) = default;
		SamplerState& operator = (SamplerState&&) = default;

		bool operator == (const SamplerState&) const;
		bool operator != (const SamplerState&) const;

		VkFilter m_VkFilter = VK_FILTER_LINEAR;
		VkSamplerAddressMode m_VkAddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		VkSamplerMipmapMode m_VkMipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	};

	class Sampler {
	public:
		Sampler(const SamplerState& state, VkSampler vkSampler);

		bool operator == (const SamplerState& state) const;
		bool operator != (const SamplerState& state) const;

		auto GetVkSampler() const->VkSampler;

	private:
		SamplerState m_State;
		VkSampler m_VkSampler;
	};
}