#pragma once

#include <DwarfConstructorRender/Types.h>
#include <DwarfConstructorRender/VertexAttributeRegistry.h>

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