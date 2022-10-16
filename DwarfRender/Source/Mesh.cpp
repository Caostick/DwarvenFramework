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