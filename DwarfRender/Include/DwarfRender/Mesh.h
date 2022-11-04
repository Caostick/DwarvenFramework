#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Mesh {
	public:
		virtual ~Mesh() {}

		virtual void SetName(const df::StringView& name) = 0;
		virtual void Create(uint32 vertexCount, uint32 indexCount) = 0;
		virtual bool EnableAttribute(const df::StringView& attributeName, const void* data = nullptr, uint32 size = 0) = 0;
		virtual bool SetAttributeData(const df::StringView& attributeName, const void* data, uint32 size, uint32 offset = 0) = 0;
		virtual bool SetIndexData(const uint32* data, uint32 size, uint32 offset = 0) = 0;
		virtual auto GetVertexCount() const->uint32 = 0;
		virtual auto GetIndexCount() const->uint32 = 0;
	};
}