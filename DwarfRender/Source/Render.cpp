#include <DwarfRender/Render.h>

#ifdef VK_RENDER_IMPLEMENTATION
#include "Vulkan/VkRenderer.h"
#endif

#include <DwarvenCore/New.h>

auto df::CreateRenderer(df::Window& window)->df::Renderer* {
#ifdef VK_RENDER_IMPLEMENTATION
	return DFNew vk::Renderer(window);
#endif
}