#include "VkScopedRenderEvent.h"

#include <DwarvenCore/Memory.h>

vk::ScopedRenderEvent::ScopedRenderEvent(VkCommandBuffer rcb, const char* name)
	: m_VkCommandBuffer(rcb) {

#if DF_USE_DEBUG_MARKERS
	VkDebugMarkerMarkerInfoEXT markerInfo = {};
	markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
	float color[4] = { 0.5f, 0.5f, 1.0f, 1.0f };
	memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
	markerInfo.pMarkerName = name;
	vk::API::CmdDebugMarkerBeginEXT(m_VkCommandBuffer, &markerInfo);
#endif
}

vk::ScopedRenderEvent::ScopedRenderEvent(vk::ScopedRenderEvent&& other) noexcept
	: m_VkCommandBuffer(other.m_VkCommandBuffer) {
	other.m_VkCommandBuffer = VK_NULL_HANDLE;
}

vk::ScopedRenderEvent& vk::ScopedRenderEvent::operator = (vk::ScopedRenderEvent&& other) noexcept {
	m_VkCommandBuffer = other.m_VkCommandBuffer;
	other.m_VkCommandBuffer = VK_NULL_HANDLE;

	return *this;
}

vk::ScopedRenderEvent::~ScopedRenderEvent() {
#if DF_USE_DEBUG_MARKERS
	if (m_VkCommandBuffer != VK_NULL_HANDLE) {
		vk::API::CmdDebugMarkerEndEXT(m_VkCommandBuffer);
	}
#endif
}