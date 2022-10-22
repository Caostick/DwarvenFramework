#pragma once

#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>

namespace rf {
	class Mesh {
	public:
		Mesh(uint32 vertexCount, uint32 indexCount);

	public:
		uint32 m_VertexCount;
		uint32 m_IndexCount;

		df::Vector<BufferId> m_VertexAttributeBuffers;
		BufferId m_IndexBuffer;
	};
}