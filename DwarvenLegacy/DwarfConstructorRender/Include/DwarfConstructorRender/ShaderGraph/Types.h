#pragma once

#include <DwarvenCore/String.h>

namespace rf {
	namespace sg {
		enum class EShaderPass {
			Vertex,
			Fragment
		};

		enum class EPipelineType {
			Graphics,
			Compute
		};

		enum class EVariableType {
			Unknown,
			Float,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4
		};

		enum class EVectorComp {
			None,
			X,
			Y,
			Z,
			W
		};

		auto ToString(EVariableType type) ->df::String;
	}
}