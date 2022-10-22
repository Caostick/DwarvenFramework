#pragma once

#include <DwarfRender/ParameterSet.h>

namespace vk {
	class RenderCore;
	class ParameterSetDefinition;
}

namespace vk {
	class ParameterSet : public df::ParameterSet {
	public:
		ParameterSet(RenderCore& renderCore, ParameterSetDefinition* definition);
		~ParameterSet();

		virtual void DeclareFloatParameter(const df::StringView& name, float defaultValue) override;
		virtual void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) override;
		virtual void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) override;
		virtual void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) override;
		virtual void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) override;
		virtual void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) override;
		virtual void DeclareTextureParameter(const df::StringView& name) override;
		virtual void DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) override;

	public:
		auto GetDefinition() const->ParameterSetDefinition*;

	private:
		RenderCore& m_RenderCore;
		ParameterSetDefinition* m_Definition;
	};
}