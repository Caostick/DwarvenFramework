#pragma once

#include <DwarfRender/DataBufferRegistry.h>

namespace rf {
	struct DataBuffer {
		// Buffer element type
		using DataType = void;

		// Buffer name for debug and bindings
		static const char* Name;
	};
}