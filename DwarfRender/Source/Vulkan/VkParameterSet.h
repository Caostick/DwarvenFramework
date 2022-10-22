#pragma once

#include <DwarfRender/ParameterSet.h>

namespace vk {
	class RenderCore;
	class ParameterSetDefinition;
}

namespace vk {
	class ParameterSet : public df::ParameterSet {
	public:
		ParameterSet(RenderCore& renderCore, ParameterSetDefinition& definition);
		~ParameterSet();

		virtual void DeclareFloatParameter(const df::StringView& name, float defaultValue) override;
		virtual void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) override;
		virtual void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) override;
		virtual void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) override;
		virtual void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) override;
		virtual void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) override;
		virtual void DeclareTextureParameter(const df::StringView& name) override;
		virtual void DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) override;

		virtual bool HasFloatParameter(const df::StringView& name) override;
		virtual bool HasVec2Parameter(const df::StringView& name) override;
		virtual bool HasVec3Parameter(const df::StringView& name) override;
		virtual bool HasVec4Parameter(const df::StringView& name) override;
		virtual bool HasMat3Parameter(const df::StringView& name) override;
		virtual bool HasMat4Parameter(const df::StringView& name) override;
		virtual bool HasTextureParameter(const df::StringView& name) override;
		virtual bool HasBufferParameter(const df::StringView& name) override;

		virtual bool Set(const df::StringView& name, float value) override;
		virtual bool Set(const df::StringView& name, const Vec2& value) override;
		virtual bool Set(const df::StringView& name, const Vec3& value) override;
		virtual bool Set(const df::StringView& name, const Vec4& value) override;
		virtual bool Set(const df::StringView& name, const Mat3& value) override;
		virtual bool Set(const df::StringView& name, const Mat4& value) override;
		virtual bool Set(const df::StringView& name, df::Texture* texture) override;
		virtual bool Set(const df::StringView& name, df::Buffer* buffer) override;

	public:
		auto GetDefinition() const->ParameterSetDefinition&;

	private:
		RenderCore& m_RenderCore;
		ParameterSetDefinition& m_Definition;
	};
}