#pragma once

#include <DwarfRender/SamplerState.h>

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
	class Texture;
	class Buffer;
}

namespace df {
	enum class EShaderConstantType {
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4
	};
}

namespace df {
	class ParameterSet {
	public:
		virtual ~ParameterSet() {}

		virtual void DeclareFloatParameter(const df::StringView& name, float defaultValue) = 0;
		virtual void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) = 0;
		virtual void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) = 0;
		virtual void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) = 0;
		virtual void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) = 0;
		virtual void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) = 0;
		virtual void DeclareTextureParameter(const df::StringView& name) = 0;
		virtual void DeclareBufferParameter(const df::StringView& name, EShaderConstantType dataType) = 0;

		virtual bool HasFloatParameter(const df::StringView& name) = 0;
		virtual bool HasVec2Parameter(const df::StringView& name) = 0;
		virtual bool HasVec3Parameter(const df::StringView& name) = 0;
		virtual bool HasVec4Parameter(const df::StringView& name) = 0;
		virtual bool HasMat3Parameter(const df::StringView& name) = 0;
		virtual bool HasMat4Parameter(const df::StringView& name) = 0;
		virtual bool HasTextureParameter(const df::StringView& name) = 0;
		virtual bool HasBufferParameter(const df::StringView& name) = 0;

		virtual bool SetFloat(const df::StringView& name, float value) = 0;
		virtual bool SetVec2(const df::StringView& name, const Vec2& value) = 0;
		virtual bool SetVec3(const df::StringView& name, const Vec3& value) = 0;
		virtual bool SetVec4(const df::StringView& name, const Vec4& value) = 0;
		virtual bool SetMat3(const df::StringView& name, const Mat3& value) = 0;
		virtual bool SetMat4(const df::StringView& name, const Mat4& value) = 0;
		virtual bool SetBuffer(const df::StringView& name, df::Buffer* buffer) = 0;
		virtual bool SetTexture(const df::StringView& name, df::Texture* texture) = 0;
		virtual bool SetFilter(const df::StringView& textureName, df::EFilter filter) = 0;
		virtual bool SetMipMapMode(const df::StringView& textureName, df::EMipmapMode mipMapMode) = 0;
		virtual bool SetAddressMode(const df::StringView& textureName, df::EAddressMode addressMode) = 0;
	};
}