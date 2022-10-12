#include "Mesh.h"

#include <DwarfRender/VertexAttribute.h>

#include <DwarvenCore/Assert.h>

rf::Mesh::Mesh(uint32 vertexCount, uint32 indexCount)
	: m_VertexCount(vertexCount)
	, m_IndexCount(indexCount) {

	m_VertexAttributeBuffers.resize(rf::VertexAttributeRegistry::TypeCount);
	for (auto& attr : m_VertexAttributeBuffers) {
		attr = nullptr;
	}
}

auto rf::Mesh::SetAttributeBuffer(uint32 attrId, BufferId buffer) {
	DFAssert(attrId < m_VertexAttributeBuffers.size(), "Vetex arrtibute index is out of rane!");

	m_VertexAttributeBuffers[attrId] = buffer;
}

auto rf::Mesh::SetIndexBuffer(BufferId buffer) {
	m_IndexBuffer = buffer;
}

auto rf::Mesh::GetVertexCount() const->uint32 {
	return m_VertexCount;
}

auto rf::Mesh::GetIndexCount() const->uint32 {
	return m_IndexCount;
}

auto rf::Mesh::GetAttributeBuffer(uint32 attrId) const->BufferId {
	DFAssert(attrId < m_VertexAttributeBuffers.size(), "Vetex arrtibute index is out of rane!");

	return m_VertexAttributeBuffers[attrId];
}

auto rf::Mesh::GetIndexBuffer() const->BufferId {
	return m_IndexBuffer;
}