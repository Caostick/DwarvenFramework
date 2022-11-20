#pragma once

#include <DwarfConstructorRender/RasterizationState.h>
#include <DwarfConstructorRender/BlendState.h>
#include <DwarfConstructorRender/DepthState.h>
#include <DwarfConstructorRender/ObjectId.h>
#include <DwarfConstructorRender/Types.h>

namespace rf {
	class RenderCore;
}

namespace rf {
	enum class ETexture {
		Null,
		White,
		Black,

		COUNT
	};

	enum class ESamplerState {
		Nearest,
		Linear,

		COUNT
	};

	class GlobalObjects {
	public:
		static void Create(rf::RenderCore& renderCore);
		static void Destroy(rf::RenderCore& renderCore);

		static auto Get(rf::ERasterizationState state)->rf::RasterizationStateId;
		static auto Get(rf::EBlendState state)->rf::BlendStateId;
		static auto Get(rf::EDepthStencilState state)->rf::DepthStateId;
		static auto Get(rf::ETexture texture)->rf::TextureId;
		static auto Get(rf::ESamplerState samplerState)->rf::SamplerId;
	};
}