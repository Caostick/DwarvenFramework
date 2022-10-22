#include "VkScopedRenderEvent.h"

#include <DwarvenCore/Memory.h>

rf::api::ScopedRenderEvent::ScopedRenderEvent(VkCommandBuffer rcb, const char* name)
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

rf::api::ScopedRenderEvent::ScopedRenderEvent(rf::api::ScopedRenderEvent&& other) noexcept
	: m_VkCommandBuffer(other.m_VkCommandBuffer) {
	other.m_VkCommandBuffer = VK_NULL_HANDLE;
}

rf::api::ScopedRenderEvent& rf::api::ScopedRenderEvent::operator = (rf::api::ScopedRenderEvent&& other) noexcept {
	m_VkCommandBuffer = other.m_VkCommandBuffer;
	other.m_VkCommandBuffer = VK_NULL_HANDLE;

	return *this;
}

rf::api::ScopedRenderEvent::~ScopedRenderEvent() {
#if DF_USE_DEBUG_MARKERS
	if (m_VkCommandBuffer != VK_NULL_HANDLE) {
		vk::API::CmdDebugMarkerEndEXT(m_VkCommandBuffer);
	}
#endif
}