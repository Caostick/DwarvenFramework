#pragma once

#include <DwarvenCore/StringView.h>

namespace df {
	class Buffer {
	public:
		virtual ~Buffer() {}

		virtual void SetName(const df::StringView& name) = 0;

		virtual void Create(uint32 size) = 0;
		virtual void SetData(void* data, uint32 size, uint32 offset = 0) = 0;

	};
}