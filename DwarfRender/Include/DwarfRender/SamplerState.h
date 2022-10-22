#pragma once

namespace df {
	enum class EFilter {
		Nearest,
		Linear
	};

	enum class EMipmapMode {
		Nearest,
		Linear
	};

	enum class EAddressMode {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge
	};
}