#include "VkVertexDescription.h"

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Memory.h>

bool vk::VertexDescription::operator == (const VertexDescription& other) const {
	const size_t ad1 = m_AttributeDescriptions.size();
	const size_t ad2 = other.m_AttributeDescriptions.size();
	const size_t bd1 = m_BindingDescriptions.size();
	const size_t bd2 = other.m_BindingDescriptions.size();

	return
		(ad1 == ad2) && (bd1 == bd2) &&
		!df::TMemCmp(m_AttributeDescriptions.data(), other.m_AttributeDescriptions.data(), ad1) &&
		!df::TMemCmp(m_BindingDescriptions.data(), other.m_BindingDescriptions.data(), bd1);
}

bool vk::VertexDescription::operator != (const VertexDescription& other) const {
	return !(*this == other);
}

void vk::VertexDescription::AddBindingDescription(uint32 index, uint32 stride, VkVertexInputRate inputRate) {
	VkVertexInputBindingDescription binding = {};
	binding.binding = index;
	binding.stride = stride;
	binding.inputRate = inputRate;

	m_BindingDescriptions.emplace_back(binding);
}

void vk::VertexDescription::AddAttributeDescription(uint32 location, uint32 binding, VkFormat format, uint32 offset) {
	VkVertexInputAttributeDescription attribute = {};
	attribute.binding = binding;
	attribute.location = location;
	attribute.format = format;
	attribute.offset = offset;

	m_AttributeDescriptions.emplace_back(attribute);
}

auto vk::VertexDescription::GetBindingDescriptionCount() const -> uint32 {
	return static_cast<uint32>(m_BindingDescriptions.size());
}

auto vk::VertexDescription::GetAttributeDescriptionCount() const -> uint32 {
	return static_cast<uint32>(m_AttributeDescriptions.size());
}

auto vk::VertexDescription::GetAttributeDescriptions() const -> const VkVertexInputAttributeDescription* {
	return m_AttributeDescriptions.data();
}

auto vk::VertexDescription::GetBindingDescriptions() const -> const VkVertexInputBindingDescription* {
	return m_BindingDescriptions.data();
}
