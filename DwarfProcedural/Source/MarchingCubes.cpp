#include <DwarfProcedural/MarchingCubes.h>
#include <DwarfProcedural/LookupTable.h>

auto df::MarchingCubes::Generate(
	const DepthFunction& func,
	const Vec3& min,
	const Vec3& max,
	const IVec3& dim,
	Vec3* vertexData) -> uint32 {

	const Vec3 step = (max - min) / Vec3(dim);
	const Vec3* cubeVertices = df::mc::CubeVertices();

	Vec3 points[8] = {};
	float weights[8] = {};
	uint32 vertexCount = 0;

	for (int32 x = 0; x < dim.X; ++x) {
		for (int32 y = 0; y < dim.Y; ++y) {
			for (int32 z = 0; z < dim.Z; ++z) {
				for (int32 i = 0; i < 8; ++i) {
					points[i] = (Vec3(float(x), float(y), float(z)) + cubeVertices[i]) * step;
					weights[i] = func.Calculate(points[i]);
				}

				vertexCount += GenerateVertices(
					weights, points, vertexData
				);
			}
		}
	}

	return vertexCount;
}

auto df::MarchingCubes::GenerateVertices(
	const float* w,
	const Vec3* v,
	Vec3* vertices)->uint32 {

	const uint8 cubeCase = df::mc::GetCubeCase(w);
	const uint32 numVertices = static_cast<uint32>(df::mc::CaseToNumVertices(cubeCase));

	for (uint32 i = 0; i < numVertices; ++i) {
		const auto edgeIdx = df::mc::CaseToTriangleList(cubeCase)[i];

		const auto idx0 = df::mc::EdgeToVertIndices(edgeIdx)[0];
		const auto idx1 = df::mc::EdgeToVertIndices(edgeIdx)[1];

		const auto w0 = w[idx0];
		const auto w1 = w[idx1];

		const auto& v0 = v[idx0];
		const auto& v1 = v[idx1];

		const float l = -w0 / (w1 - w0);
		vertices[i] = (v1 - v0) * l + v0;
	}

	return numVertices;
}