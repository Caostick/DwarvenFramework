#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/EnumFlags.h>
#include <DwarvenCore/StringView.h>

namespace df {
	enum class ETextureFormat {
		Undefined,

		D32_Float,
		D24_Float_S8_UInt,

		R8_UNorm,
		R16_UNorm,
		R8G8_UNorm,
		R8G8B8A8_UNorm,
		A2R10G10B10_UNorm,

		R16_Float,
		R32_Float,
		R16G16_Float,
		R16G16B16A16_Float,

		COUNT
	};

	enum class EImageUsageFlag {
		Texture = 1 << 0,
		RenderTarget = 1 << 1,
		DepthRenderTarget = 1 << 2,
		Mips = 1 << 3,
	};

	using ImageUsageFlags = df::EnumFlags<EImageUsageFlag>;
	DF_DEFINE_ENUM_FLAG_OPERATORS(ImageUsageFlags);
}

namespace df {
	class Texture {
	public:
		virtual ~Texture() {}

		virtual void SetName(const df::StringView& name) = 0;
		virtual void Create(uint32 width, uint32 height, ETextureFormat format, ImageUsageFlags usage = EImageUsageFlag::Texture) = 0;
		virtual void SetData(void* data, uint32 size) = 0;
		virtual void GenerateMips() = 0;
		virtual auto GetWidth() const->uint32 = 0;
		virtual auto GetHeight() const->uint32 = 0;
		virtual auto GetMips() const->uint32 = 0;
	};
}