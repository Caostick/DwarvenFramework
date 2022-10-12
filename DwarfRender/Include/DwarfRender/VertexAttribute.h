#pragma once

#include <DwarfRender/Types.h>
#include <DwarfRender/VertexAttributeRegistry.h>

namespace rf {
	struct VertexAttribute {
		// VertexAttribute name for bindings
		static const char* Name;

		// VertexAttribute format
		static rf::EVertexAttributeFormat Format;

		// VertexAttribute data type
		using DataType = void;
	};
}