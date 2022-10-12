#pragma once

#include <DwarfRender/Types.h>

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Math/Math.h>

namespace rf {
	class ParamSetDefinition;
}

namespace rf {
	class ParamSetDeclarator {
	public:
		ParamSetDeclarator(ParamSetDefinition& parameters);

		void DeclareFloatParameter(const df::StringView& name, float defaultValue);
		void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue);
		void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue);
		void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue);
		void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue);
		void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue);
		void DeclareTextureParameter(const df::StringView& name, const df::StringView& defaultTextureName = "");
		void DeclareBufferParameter(const df::StringView& name, rf::EShaderParameterType dataType, bool multiframed = false);
	private:
		ParamSetDefinition& m_Parameters;
	};
}