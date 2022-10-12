#include <DwarfRender/ParamSetDeclarator.h>
#include <DwarfRender/ParamSetDefinition.h>

rf::ParamSetDeclarator::ParamSetDeclarator(ParamSetDefinition& parameters)
	: m_Parameters(parameters) {}

void rf::ParamSetDeclarator::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	m_Parameters.DeclareFloatParameter(name, defaultValue);
}
 
void rf::ParamSetDeclarator::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	m_Parameters.DeclareVec2Parameter(name, defaultValue);
}

void rf::ParamSetDeclarator::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	m_Parameters.DeclareVec3Parameter(name, defaultValue);
}

void rf::ParamSetDeclarator::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	m_Parameters.DeclareVec4Parameter(name, defaultValue);
}

void rf::ParamSetDeclarator::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	m_Parameters.DeclareMat3Parameter(name, defaultValue);
}

void rf::ParamSetDeclarator::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	m_Parameters.DeclareMat4Parameter(name, defaultValue);
}

void rf::ParamSetDeclarator::DeclareTextureParameter(const df::StringView& name, const df::StringView& defaultTextureName /*= ""*/) {
	m_Parameters.DeclareTextureParameter(name, defaultTextureName);
}

void rf::ParamSetDeclarator::DeclareBufferParameter(const df::StringView& name, rf::EShaderParameterType dataType, bool multiframed /*= false*/) {
	m_Parameters.DeclareBufferParameter(name, dataType, multiframed);
}