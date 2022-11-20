#pragma once

#include <vulkan/vulkan.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace vk {
    class VertexDescription {
    public:
		VertexDescription() = default;
		VertexDescription(const vk::VertexDescription&) = default;
		VertexDescription(vk::VertexDescription&&) = default;
		vk::VertexDescription& operator = (const vk::VertexDescription&) = default;
		vk::VertexDescription& operator = (vk::VertexDescription&&) = default;

		bool operator == (const VertexDescription& other) const;
		bool operator != (const VertexDescription& other) const;

        void AddBindingDescription(uint32 index, uint32 stride, VkVertexInputRate inputRate);
        void AddAttributeDescription(uint32 location, uint32 binding, VkFormat format, uint32 offset = 0);

        auto GetBindingDescriptionCount() const -> uint32;
        auto GetAttributeDescriptionCount() const ->uint32;

        auto GetAttributeDescriptions() const -> const VkVertexInputAttributeDescription*;
        auto GetBindingDescriptions() const -> const VkVertexInputBindingDescription*;

    private:
        df::Vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
        df::Vector<VkVertexInputBindingDescription> m_BindingDescriptions;
    };
}