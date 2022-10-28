#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Mesh {
	public:
		virtual ~Mesh() {}

		virtual void SetName(const df::StringView& name) = 0;
		virtual void Create(uint32 vertexCount, uint32 indexCount) = 0;
		virtual bool EnableAttribute(const df::StringView& attributeName) = 0;
		virtual bool SetAttributeData(const df::StringView& attributeName, void* data, uint32 size, uint32 offset = 0) = 0;
		virtual bool SetIndexData(uint32* data, uint32 size, uint32 offset = 0) = 0;
	};
}