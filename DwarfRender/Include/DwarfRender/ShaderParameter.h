#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Math/Math.h>

namespace rf {
	struct ShaderParameter {
		static const char* Name;
	};

	struct ConstantParameter : ShaderParameter {};

	struct BufferParameter : ShaderParameter {
		using DataType = void;
		static const bool Multiframed;
	};

	struct FloatParameter : ConstantParameter {
		using DataType = float;
		static const float DefaultValue;
	};

	struct Vec2Parameter : ConstantParameter {
		using DataType = Vec2;
		static const Vec2 DefaultValue;
	};

	struct Vec3Parameter : ConstantParameter {
		using DataType = Vec3;
		static const Vec3 DefaultValue;
	};

	struct Vec4Parameter : ConstantParameter {
		using DataType = Vec4;
		static const Vec4 DefaultValue;
	};

	struct Mat3Parameter : ConstantParameter {
		using DataType = Mat3;
		static const Mat3 DefaultValue;
	};

	struct Mat4Parameter : ConstantParameter {
		using DataType = Mat4;
		static const Mat4 DefaultValue;
	};

	struct FloatBufferParameter : BufferParameter {
		using DataType = float;
	};

	struct Vec2BufferParameter : BufferParameter {
		using DataType = Vec2;
	};

	struct Vec4BufferParameter : BufferParameter {
		using DataType = Vec4;
	};

	struct TextureParameter : ShaderParameter {};
}