#include <DwarfRender/ParamSet.h>
#include <DwarfRender/ParamSetDefinition.h>

#include "RenderCore.h"

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Log.h>

rf::ParamSet::ParamSet(const ParamSetDefinition& definition) 
	: m_Definition(definition) 
	, m_PrevFrameIndex(-1) 
	, m_IsStatic(true)
	, m_ConstantBuffer(nullptr) {
}

void rf::ParamSet::Init(rf::RenderCore& renderCore, bool isStatic) {
	const auto constantBufferSize = m_Definition.GetConstantBufferSize();
	m_IsStatic = isStatic;

	if (constantBufferSize > 0) {
		m_ConstantBuffer = renderCore.CreateBuffer(
			m_Definition.GetName(),
			constantBufferSize,
			rf::EBufferUsageFlag::Uniform, EBufferAccessType::Transfer, 
			m_IsStatic
		);
	}

	m_Buffers.resize(m_Definition.GetBuffers().size());
	m_SecondaryBuffers.resize(m_Definition.GetBuffers().size());
	m_Textures.resize(m_Definition.GetTextures().size());
	m_Samplers.resize(m_Definition.GetTextures().size());

	m_DescriptorSet.resize(m_IsStatic ? 1 : renderCore.GetNumFramesInFlight());
	for (auto& ds : m_DescriptorSet) {
		ds = m_Definition.CreateDescriptorSet(renderCore);
	}

	InitDefaultValues();
}

void rf::ParamSet::Release(rf::RenderCore& renderCore) {
	for (auto& ds : m_DescriptorSet) {
		renderCore.DestroyDescriptorSet(ds);
	}
	m_DescriptorSet.clear();

	m_Buffers.clear();
	m_SecondaryBuffers.clear();
	m_Textures.clear();
	m_Samplers.clear();

	if (m_ConstantBuffer) {
		renderCore.DestroyBuffer(m_ConstantBuffer);
		m_ConstantBuffer = nullptr;
	}
}

bool rf::ParamSet::Update(uint32 frameIndex) {
	const uint32 dsIdx = m_IsStatic ? 0 : frameIndex;

	if (m_PrevFrameIndex == -1) {
		m_PrevFrameIndex = int32(frameIndex);
	}

	m_DescriptorSet[dsIdx]->Begin();

	if (m_ConstantBuffer) {
		m_DescriptorSet[dsIdx]->SetUniformBuffer(0, 0,
			m_ConstantBuffer,
			m_ConstantBuffer->m_Stride,
			m_ConstantBuffer->m_IsStatic ? 0 : (m_ConstantBuffer->m_Stride * frameIndex)
		);

		m_DescriptorSet[dsIdx]->SetUniformBuffer(0, 1,
			m_ConstantBuffer,
			m_ConstantBuffer->m_Stride,
			m_ConstantBuffer->m_IsStatic ? 0 : (m_ConstantBuffer->m_Stride * m_PrevFrameIndex)
		);
	}

	const auto& bufDefs = m_Definition.GetBuffers();
	for (size_t i = 0; i < bufDefs.size(); ++i) {
		if (m_Buffers[i] == nullptr) {
			df::log::WarningFmt("ParameterSet(%s) buffer(%s) wasn't set!",
				m_Definition.GetName().c_str(),
				bufDefs[i].m_Name.c_str());
			return false;
		}

		m_DescriptorSet[dsIdx]->SetStorageBuffer(
			bufDefs[i].m_Binding,
			0,
			m_Buffers[i],
			m_Buffers[i]->m_Stride,
			m_Buffers[i]->m_IsStatic ? 0 : (m_Buffers[i]->m_Stride * frameIndex)
		);

		if (bufDefs[i].m_Multiframed) {
			if (m_SecondaryBuffers[i] == nullptr) {
				df::log::WarningFmt("ParameterSet(%s) secondary buffer(%s) wasn't set!",
					m_Definition.GetName().c_str(),
					bufDefs[i].m_Name.c_str());
				return false;
			}

			if (m_Buffers[i]->m_Size != m_SecondaryBuffers[i]->m_Size) {
				df::log::WarningFmt("ParameterSet(%s) buffer(%s)'s size and its secondary size don't match!",
					m_Definition.GetName().c_str(),
					bufDefs[i].m_Name.c_str());
				return false;
			}

			m_DescriptorSet[dsIdx]->SetStorageBuffer(
				bufDefs[i].m_Binding,
				1,
				m_SecondaryBuffers[i],
				m_SecondaryBuffers[i]->m_Stride,
				m_SecondaryBuffers[i]->m_IsStatic ? 0 : (m_SecondaryBuffers[i]->m_Stride * frameIndex)
			);
		}
	}

	const auto& texDefs = m_Definition.GetTextures();
	for (size_t i = 0; i < texDefs.size(); ++i) {
		if (m_Textures[i] == nullptr) {
			df::log::WarningFmt("ParameterSet(%s) texture(%s) wasn't set!",
				m_Definition.GetName().c_str(),
				texDefs[i].m_Name.c_str());

			return false;
		}

		if (m_Samplers[i] == nullptr) {
			df::log::WarningFmt("ParameterSet(%s) sampler(%s) wasn't set!",
				m_Definition.GetName().c_str(),
				texDefs[i].m_Name.c_str());
			return false;
		}

		m_DescriptorSet[dsIdx]->SetCombinedImageSampler(
			texDefs[i].m_Binding,
			0,
			m_Textures[i],
			m_Samplers[i]
		);
	}

	m_DescriptorSet[dsIdx]->End();

	m_PrevFrameIndex = int32(frameIndex);

	return true;
}

void rf::ParamSet::SetFloatByOffset(uint32 constOffset, float value) {
	float* ptr = reinterpret_cast<float*>(m_ConstantBuffer->m_Data + constOffset);
	*ptr = value;
}

bool rf::ParamSet::SetFloatByName(const df::StringView& name, float value) {
	for (auto& c : m_Definition.GetFloatConstants()) {
		if (c.m_Name == name) {
			SetFloatByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetFloatByOffset(uint32 constOffset) const -> float {
	const float* ptr = reinterpret_cast<float*>(m_ConstantBuffer->m_Data + constOffset);
	return *ptr;
}

auto rf::ParamSet::GetFloatByName(const df::StringView& name) const -> float {
	for (auto& c : m_Definition.GetFloatConstants()) {
		if (c.m_Name == name) {
			return GetFloatByOffset(c.m_Offset);
		}
	}

	return 0.0f;
}

void rf::ParamSet::SetVec2ByOffset(uint32 constOffset, const Vec2& value) {
	Vec2* ptr = reinterpret_cast<Vec2*>(m_ConstantBuffer->m_Data + constOffset);
	*ptr = value;
}

bool rf::ParamSet::SetVec2ByName(const df::StringView& name, const Vec2& value) {
	for (auto& c : m_Definition.GetVec2Constants()) {
		if (c.m_Name == name) {
			SetVec2ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetVec2ByOffset(uint32 constOffset) const -> const Vec2& {
	const Vec2* ptr = reinterpret_cast<Vec2*>(m_ConstantBuffer->m_Data + constOffset);
	return *ptr;
}

auto rf::ParamSet::GetVec2ByName(const df::StringView& name) const -> Vec2 {
	for (auto& c : m_Definition.GetVec2Constants()) {
		if (c.m_Name == name) {
			return GetVec2ByOffset(c.m_Offset);
		}
	}
	return Vec2();
}

void rf::ParamSet::SetVec3ByOffset(uint32 constOffset, const Vec3& value) {
	Vec3* ptr = reinterpret_cast<Vec3*>(m_ConstantBuffer->m_Data + constOffset);
	*ptr = value;
}

bool rf::ParamSet::SetVec3ByName(const df::StringView& name, const Vec3& value) {
	for (auto& c : m_Definition.GetVec3Constants()) {
		if (c.m_Name == name) {
			SetVec3ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetVec3ByOffset(uint32 constOffset) const -> const Vec3& {
	const Vec3* ptr = reinterpret_cast<Vec3*>(m_ConstantBuffer->m_Data + constOffset);
	return *ptr;
}

auto rf::ParamSet::GetVec3ByName(const df::StringView& name) const -> Vec3 {
	for (auto& c : m_Definition.GetVec3Constants()) {
		if (c.m_Name == name) {
			return GetVec3ByOffset(c.m_Offset);
		}
	}
	return Vec3();
}

void rf::ParamSet::SetVec4ByOffset(uint32 constOffset, const Vec4& value) {
	Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstantBuffer->m_Data + constOffset);
	*ptr = value;
}

bool rf::ParamSet::SetVec4ByName(const df::StringView& name, const Vec4& value) {
	for (auto& c : m_Definition.GetVec4Constants()) {
		if (c.m_Name == name) {
			SetVec4ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetVec4ByOffset(uint32 constOffset) const -> const Vec4& {
	const Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstantBuffer->m_Data + constOffset);
	return *ptr;
}

auto rf::ParamSet::GetVec4ByName(const df::StringView& name) const -> Vec4 {
	for (auto& c : m_Definition.GetVec4Constants()) {
		if (c.m_Name == name) {
			return GetVec4ByOffset(c.m_Offset);
		}
	}
	return Vec4();
}

void rf::ParamSet::SetMat3ByOffset(uint32 constOffset, const Mat3& value) {
	Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstantBuffer->m_Data + constOffset);

	ptr[0].X = value.M[0];
	ptr[0].Y = value.M[1];
	ptr[0].Z = value.M[2];
	ptr[1].X = value.M[3];
	ptr[1].Y = value.M[4];
	ptr[1].Z = value.M[5];
	ptr[2].X = value.M[6];
	ptr[2].Y = value.M[7];
	ptr[2].Z = value.M[8];
}

bool rf::ParamSet::SetMat3ByName(const df::StringView& name, const Mat3& value) {
	for (auto& c : m_Definition.GetMat3Constants()) {
		if (c.m_Name == name) {
			SetMat3ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetMat3ByOffset(uint32 constOffset) const->Mat3 {
	const Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstantBuffer->m_Data + constOffset);

	Mat3 result;
	result.M[0] = ptr[0].X;
	result.M[1] = ptr[0].Y;
	result.M[2] = ptr[0].Z;
	result.M[3] = ptr[1].X;
	result.M[4] = ptr[1].Y;
	result.M[5] = ptr[1].Z;
	result.M[6] = ptr[2].X;
	result.M[7] = ptr[2].Y;
	result.M[8] = ptr[2].Z;

	return result;
}

auto rf::ParamSet::GetMat3ByName(const df::StringView& name) const -> Mat3 {
	for (auto& c : m_Definition.GetMat3Constants()) {
		if (c.m_Name == name) {
			return GetMat3ByOffset(c.m_Offset);
		}
	}
	return Mat3();
}

void rf::ParamSet::SetMat4ByOffset(uint32 constOffset, const Mat4& value) {
	Mat4* ptr = reinterpret_cast<Mat4*>(m_ConstantBuffer->m_Data + constOffset);
	*ptr = value;
}

bool rf::ParamSet::SetMat4ByName(const df::StringView& name, const Mat4& value) {
	for (auto& c : m_Definition.GetMat4Constants()) {
		if (c.m_Name == name) {
			SetMat4ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

auto rf::ParamSet::GetMat4ByOffset(uint32 constOffset) const -> const Mat4& {
	const Mat4* ptr = reinterpret_cast<Mat4*>(m_ConstantBuffer->m_Data + constOffset);
	return *ptr;
}

auto rf::ParamSet::GetMat4ByName(const df::StringView& name) const -> Mat4 {
	for (auto& c : m_Definition.GetMat4Constants()) {
		if (c.m_Name == name) {
			return GetMat4ByOffset(c.m_Offset);
		}
	}
	return Mat4();
}

void rf::ParamSet::SetBufferByIndex(uint32 index, rf::BufferId buffer, bool isHistory /*= false*/) {
	if (!isHistory) {
		m_Buffers[index] = buffer;
	} else {
		m_SecondaryBuffers[index] = buffer;
	}
}

bool rf::ParamSet::SetBufferByName(const df::StringView& name, rf::BufferId buffer, bool isHistory /*= false*/) {
	auto buffers = m_Definition.GetBuffers();

	for (size_t i = 0; i < buffers.size(); ++i) {
		if (buffers[i].m_Name == name) {
			SetBufferByIndex(uint32(i), buffer, isHistory);
			return true;
		}
	}

	return false;
}

auto rf::ParamSet::GetBufferByIndex(uint32 index, bool isHistory /*= false*/) const->rf::BufferId {
	if (!isHistory) {
		return m_Buffers[index];
	} else {
		return m_SecondaryBuffers[index];
	}
}

auto rf::ParamSet::GetBufferByName(const df::StringView& name, bool isHistory /*= false*/) const->rf::BufferId {
	auto buffers = m_Definition.GetBuffers();

	for (size_t i = 0; i < buffers.size(); ++i) {
		if (buffers[i].m_Name == name) {
			return GetBufferByIndex(uint32(i), isHistory);
		}
	}

	return nullptr;
}

void rf::ParamSet::SetTextureByIndex(uint32 index, rf::TextureId texture) {
	m_Textures[index] = texture;
}

bool rf::ParamSet::SetTextureByName(const df::StringView& name, rf::TextureId texture) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == name) {
			SetTextureByIndex(uint32(i), texture);
			return true;
		}
	}

	return false;
}

auto rf::ParamSet::GetTextureByIndex(uint32 index) const->rf::TextureId {
	return m_Textures[index];
}

auto rf::ParamSet::GetTextureByName(const df::StringView& name) const->rf::TextureId {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == name) {
			return GetTextureByIndex(uint32(i));
		}
	}

	return nullptr;
}

void rf::ParamSet::SetSamplerByIndex(uint32 index, rf::SamplerId sampler) {
	m_Samplers[index] = sampler;
}

bool rf::ParamSet::SetSamplerByName(const df::StringView& name, rf::SamplerId sampler) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == name) {
			SetSamplerByIndex(uint32(i), sampler);
			return true;
		}
	}

	return false;
}

auto rf::ParamSet::GetSamplerByIndex(uint32 index) const->rf::SamplerId {
	return m_Samplers[index];
}

auto rf::ParamSet::GetSamplerByName(const df::StringView& name) const->rf::SamplerId {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == name) {
			return GetSamplerByIndex(uint32(i));
		}
	}

	return nullptr;
}

void rf::ParamSet::InitDefaultValues() {
	for (auto c : m_Definition.GetFloatConstants()) {
		auto data = reinterpret_cast<float*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}

	for (auto c : m_Definition.GetVec2Constants()) {
		auto data = reinterpret_cast<Vec2*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}

	for (auto c : m_Definition.GetVec3Constants()) {
		auto data = reinterpret_cast<Vec3*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}

	for (auto c : m_Definition.GetVec4Constants()) {
		auto data = reinterpret_cast<Vec4*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}

	for (auto c : m_Definition.GetMat3Constants()) {
		auto data = reinterpret_cast<Mat3*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}

	for (auto c : m_Definition.GetMat4Constants()) {
		auto data = reinterpret_cast<Mat4*>(m_ConstantBuffer->m_Data + c.m_Offset);
		*data = c.m_DefaultValue;
	}
}