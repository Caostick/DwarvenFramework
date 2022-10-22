#include "VkParameterSet.h"
#include "VkParameterSetDefinition.h"
#include "VkRenderCore.h"

vk::ParameterSet::ParameterSet(RenderCore& renderCore, ParameterSetDefinition& definition)
	: m_RenderCore(renderCore)
	, m_Definition(definition) {
	m_Definition.IncrementRefCount();
}

vk::ParameterSet::~ParameterSet() {
	if (m_Definition.DecrementRefCount() == 0) {
		m_RenderCore.DestroyParameterSetDefinition(&m_Definition);
	}
}

void vk::ParameterSet::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	m_Definition.DeclareFloatParameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	m_Definition.DeclareVec2Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	m_Definition.DeclareVec3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	m_Definition.DeclareVec4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	m_Definition.DeclareMat3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	m_Definition.DeclareMat4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareTextureParameter(const df::StringView& name) {
	m_Definition.DeclareTextureParameter(name);
}

void vk::ParameterSet::DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) {
	m_Definition.DeclareBufferParameter(name, dataType);
}

bool vk::ParameterSet::HasFloatParameter(const df::StringView& name) {
	return m_Definition.HasFloatParameter(name);
}

bool vk::ParameterSet::HasVec2Parameter(const df::StringView& name) {
	return m_Definition.HasVec2Parameter(name);
}

bool vk::ParameterSet::HasVec3Parameter(const df::StringView& name) {
	return m_Definition.HasVec3Parameter(name);
}

bool vk::ParameterSet::HasVec4Parameter(const df::StringView& name) {
	return m_Definition.HasVec4Parameter(name);
}

bool vk::ParameterSet::HasMat3Parameter(const df::StringView& name) {
	return m_Definition.HasMat3Parameter(name);
}

bool vk::ParameterSet::HasMat4Parameter(const df::StringView& name) {
	return m_Definition.HasMat4Parameter(name);
}

bool vk::ParameterSet::HasTextureParameter(const df::StringView& name) {
	return m_Definition.HasTextureParameter(name);
}

bool vk::ParameterSet::HasBufferParameter(const df::StringView& name) {
	return m_Definition.HasBufferParameter(name);
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, float /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, const Vec2& /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, const Vec3& /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, const Vec4& /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, const Mat3& /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, const Mat4& /*value*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, df::Texture* /*texture*/) {

	return false;
}

bool vk::ParameterSet::Set(const df::StringView& /*name*/, df::Buffer* /*buffer*/) {

	return false;
}

auto vk::ParameterSet::GetDefinition() const->ParameterSetDefinition& {
	return m_Definition;
}