#include "VkMesh.h"
#include "VkRenderCore.h"
#include "VkVertexAttribute.h"

vk::Mesh::Mesh(vk::RenderCore& renderCore)
	: m_RenderCore(renderCore) 
	, m_VertexCount(0)
	, m_IndexCount(0)
	, m_IndexBuffer(nullptr) {

}

vk::Mesh::~Mesh() {
	if (m_IndexBuffer) {
		m_RenderCore.DestroyBuffer(m_IndexBuffer);
	}

	for (auto& slot : m_Attributes) {
		m_RenderCore.DestroyBuffer(slot.m_Buffer);
	}
}

void vk::Mesh::SetName(const df::StringView& name) {
	m_Name = df::String(name);

	UpdateDebugNames();
}

void vk::Mesh::Create(uint32 vertexCount, uint32 indexCount) {
	m_VertexCount = vertexCount;
	m_IndexCount = indexCount;

	if (m_IndexCount != 0) {
		m_IndexBuffer = m_RenderCore.CreateBuffer();
		m_IndexBuffer->CreateBuffer(sizeof(uint32) * m_IndexCount, vk::EBufferUsageFlag::Index);
		m_IndexBuffer->SetName(m_Name);
	}
}

bool vk::Mesh::EnableAttribute(const df::StringView& attributeName, const void* data, uint32 size) {
	const auto attr = m_RenderCore.FindVertexAttribute(attributeName);
	if (!attr) {
		return false;
	}

	for (auto& slot : m_Attributes) {
		if (slot.m_Attribute == attr) {
			return true;
		}
	}

	AttributeSlot slot = {};
	slot.m_Attribute = attr;
	slot.m_Buffer = m_RenderCore.CreateBuffer();
	slot.m_Buffer->CreateBuffer(m_VertexCount * ToDataStride(attr->m_Format), EBufferUsageFlag::Vertex);
	slot.m_Buffer->SetName(m_Name);
	if (data != nullptr) {
		slot.m_Buffer->SetData(data, size);
	}

	m_VertexAttributeBits[attr->m_Index] = true;
	m_Attributes.emplace_back(slot);

	return true;
}

bool vk::Mesh::SetAttributeData(const df::StringView& attributeName, const void* data, uint32 size, uint32 offset) {
	for (auto& slot : m_Attributes) {
		if (slot.m_Attribute->m_Name == attributeName) {
			slot.m_Buffer->SetData(data, size, offset);
			return true;
		}
	}

	return false;
}

bool vk::Mesh::SetIndexData(const uint32* data, uint32 size, uint32 offset) {
	if (!m_IndexBuffer) {
		return false;
	}

	if (size + offset < m_IndexCount * sizeof(uint32)) {
		return false;
	}

	m_IndexBuffer->SetData(data, size, offset);

	return true;
}

auto vk::Mesh::GetAttributeBits() const -> const df::Bitset<64>& {
	return m_VertexAttributeBits;
}

auto vk::Mesh::GetAttributes() const->const df::Vector<AttributeSlot>& {
	return m_Attributes;
}

auto vk::Mesh::GetIndexBuffer() const -> const vk::Buffer* {
	return m_IndexBuffer;
}

auto vk::Mesh::GetVertexCount() const->uint32 {
	return m_VertexCount;
}

auto vk::Mesh::GetIndexCount() const->uint32 {
	return m_IndexCount;
}

void vk::Mesh::UpdateDebugNames() {
	if (m_IndexBuffer) {
		m_IndexBuffer->SetName(m_Name);
	}

	for (auto& slot : m_Attributes) {
		slot.m_Buffer->SetName(m_Name);
	}
}