#pragma once

namespace rf {
	enum class EFilter {
		Nearest,
		Linear,

		COUNT
	};

	enum class EMipmapMode {
		Nearest,
		Linear,
		
		COUNT
	};

	enum class EAddressMode {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge,

		COUNT
	};

	struct SamplerState {
		SamplerState() = default;
		SamplerState(SamplerState&&) = default;
		SamplerState(const SamplerState&) = default;
		SamplerState& operator = (SamplerState&&) = default;
		SamplerState& operator = (const SamplerState&) = default;

		bool operator == (const SamplerState& other) const;
		bool operator != (const SamplerState& other) const;

		EFilter m_Filter = EFilter::Nearest;
		EAddressMode m_AddressMode = EAddressMode::Repeat;
		EMipmapMode m_MipmapMode = EMipmapMode::Nearest;
	};
}