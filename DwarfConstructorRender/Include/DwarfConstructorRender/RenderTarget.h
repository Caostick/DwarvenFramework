#pragma once

#include <DwarfConstructorRender/Types.h>
#include <DwarfConstructorRender/RenderTargetRegistry.h>

namespace rf {
	struct RenderTarget {
		// RenderTarget name
		static const char* Name;

		// RenderTarget texture format
		static rf::ETextureFormat Format;
	};
}