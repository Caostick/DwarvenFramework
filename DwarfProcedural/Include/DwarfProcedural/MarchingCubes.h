#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
	class DepthFunction {
	public:
		virtual auto Calculate(const Vec3& coord) const -> float = 0;
	};

	class MarchingCubes {
	public:
		static auto Generate(
			const DepthFunction& func,
			const Vec3& min,
			const Vec3& max,
			const IVec3& dim,
			Vec3* vertexData
		) -> uint32;

		static auto GenerateVertices(
			const float* w,
			const Vec3* v,
			Vec3* vertices
		)->uint32;
	};
}