#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
	namespace mc {
		auto CubeVertices()->const Vec3*;
		auto GetCubeCase(const float* weights)->uint8;
		auto CaseToNumVertices(uint8 caseId)->uint32;
		auto CaseToTriangleList(uint8 caseId) -> const int32*;
		auto EdgeToVertIndices(uint32 edgeIdx)->const uint32*;

		bool EdgeMatchesCase(uint8 caseId, uint16 edgeIdx);
	}
}