#include "VkParameterSet.h"
#include "VkParameterSetDefinition.h"

vk::ParameterSet::ParameterSet(RenderCore& renderCore, ParameterSetDefinition* definition)
	: m_RenderCore(renderCore)
	, m_Definition(definition) {
}

vk::ParameterSet::~ParameterSet() {
}

void vk::ParameterSet::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	m_Definition->DeclareFloatParameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	m_Definition->DeclareVec2Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	m_Definition->DeclareVec3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	m_Definition->DeclareVec4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	m_Definition->DeclareMat3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	m_Definition->DeclareMat4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareTextureParameter(const df::StringView& name) {
	m_Definition->DeclareTextureParameter(name);
}

void vk::ParameterSet::DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) {
	m_Definition->DeclareBufferParameter(name, dataType);
}

auto vk::ParameterSet::GetDefinition() const->ParameterSetDefinition* {
	return m_Definition;
}