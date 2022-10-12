#pragma once

#include <DwarfRender/ObjectId.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>

namespace rf {
	class Mesh {
	public:
		Mesh(uint32 vertexCount, uint32 indexCount);

		auto SetAttributeBuffer(uint32 attrId, BufferId buffer);
		auto SetIndexBuffer(BufferId buffer);

		auto GetVertexCount() const->uint32;
		auto GetIndexCount() const->uint32;
		auto GetAttributeBuffer(uint32 attrId) const->BufferId;
		auto GetIndexBuffer() const->BufferId;

	private:
		uint32 m_VertexCount;
		uint32 m_IndexCount;

		df::Vector<BufferId> m_VertexAttributeBuffers;
		BufferId m_IndexBuffer;
	};
}