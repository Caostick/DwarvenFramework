#include "VkSampler.h"

bool vk::SamplerState::operator == (const SamplerState& other) const {
	return
		m_VkFilter == other.m_VkFilter &&
		m_VkAddressMode == other.m_VkAddressMode &&
		m_VkMipMapMode == other.m_VkMipMapMode;
}

bool vk::SamplerState::operator != (const SamplerState& other) const {
	return !(operator == (other));
}


vk::Sampler::Sampler(const SamplerState& state, VkSampler vkSampler) 
	: m_State(state)
	, m_VkSampler(vkSampler) {
}

bool vk::Sampler::operator == (const SamplerState& state) const {
	return m_State == state;
}

bool vk::Sampler::operator != (const SamplerState& state) const {
	return m_State != state;
}

auto vk::Sampler::GetVkSampler() const->VkSampler {
	return m_VkSampler;
}