#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/Types.h>

namespace df {
	enum class EVertexAttributeFormat {
		X8_Norm,
		XY8_Norm,
		XYZ8_Norm,
		XYZW8_Norm,
		X16_Norm,
		XY16_Norm,
		XYZ16_Norm,
		XYZW16_Norm,

		X8_UNorm,
		XY8_UNorm,
		XYZ8_UNorm,
		XYZW8_UNorm,
		X16_UNorm,
		XY16_UNorm,
		XYZ16_UNorm,
		XYZW16_UNorm,

		X16_Float,
		XY16_Float,
		XYZ16_Float,
		XYZW16_Float,
		X32_Float,
		XY32_Float,
		XYZ32_Float,
		XYZW32_Float,
		X64_Float,
		XY64_Float,
		XYZ64_Float,
		XYZW64_Float,

		X8_Int,
		XY8_Int,
		XYZ8_Int,
		XYZW8_Int,
		X16_Int,
		XY16_Int,
		XYZ16_Int,
		XYZW16_Int,
		X32_Int,
		XY32_Int,
		XYZ32_Int,
		XYZW32_Int,
		X64_Int,
		XY64_Int,
		XYZ64_Int,
		XYZW64_Int,

		X8_UInt,
		XY8_UInt,
		XYZ8_UInt,
		XYZW8_UInt,
		X16_UInt,
		XY16_UInt,
		XYZ16_UInt,
		XYZW16_UInt,
		X32_UInt,
		XY32_UInt,
		XYZ32_UInt,
		XYZW32_UInt,
		X64_UInt,
		XY64_UInt,
		XYZ64_UInt,
		XYZW64_UInt,

		COUNT
	};

	struct VertexAttribute {
		df::String m_Name;
		EVertexAttributeFormat m_Format = EVertexAttributeFormat::X8_UNorm;
		uint32 m_Index = 0;
	};
}