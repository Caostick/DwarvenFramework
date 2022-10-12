#pragma once

#include <DwarfRender/Types.h>
#include <DwarfRender/RenderTargetRegistry.h>

namespace rf {
	struct RenderTarget {
		// RenderTarget name
		static const char* Name;

		// RenderTarget texture format
		static rf::ETextureFormat Format;
	};
}