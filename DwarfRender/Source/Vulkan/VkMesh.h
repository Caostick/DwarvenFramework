#pragma once

#include <DwarfRender/Mesh.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Bitset.h>

namespace vk {
	class RenderCore;
	class Buffer;

	struct VertexAttribute;
}

namespace vk {
	class Mesh : public df::Mesh {
	public:
		Mesh(vk::RenderCore& renderCore);

		virtual ~Mesh() override;

		virtual void SetName(const df::StringView& name) override;
		virtual void Create(uint32 vertexCount, uint32 indexCount) override;
		virtual bool EnableAttribute(const df::StringView& attributeName, void* data, uint32 size) override;
		virtual bool SetAttributeData(const df::StringView& attributeName, void* data, uint32 size, uint32 offset) override;
		virtual bool SetIndexData(uint32* data, uint32 size, uint32 offset) override;
		virtual auto GetVertexCount() const->uint32 override;
		virtual auto GetIndexCount() const->uint32 override;

	private:
		struct AttributeSlot {
			const vk::VertexAttribute* m_Attribute = nullptr;
			vk::Buffer* m_Buffer = nullptr;
		};

	public:
		auto GetAttributes() const->const df::Vector<AttributeSlot>&;
		auto GetIndexBuffer() const -> const vk::Buffer*;

	private:
		void UpdateDebugNames();

	private:
		vk::RenderCore& m_RenderCore;

		df::String m_Name;

		uint32 m_VertexCount;
		uint32 m_IndexCount;
		df::Vector<AttributeSlot> m_Attributes;
		vk::Buffer* m_IndexBuffer;

		df::Bitset<64> m_VertexAttributeBits;
	};
}