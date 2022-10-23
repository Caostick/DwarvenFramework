#include "VkParameterSet.h"
#include "VkParameterSetDefinition.h"
#include "VkRenderCore.h"
#include "VkTexture.h"
#include "VkBuffer.h"

namespace {
	auto ToVk(df::EFilter value)->VkFilter {
		switch (value) {
		case df::EFilter::Nearest:
			return VK_FILTER_NEAREST;
		case df::EFilter::Linear:
			return VK_FILTER_LINEAR;
		default:
			return VK_FILTER_MAX_ENUM;
		}
	}

	auto ToVk(df::EMipmapMode value)->VkSamplerMipmapMode {
		switch (value) {
		case df::EMipmapMode::Nearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		case df::EMipmapMode::Linear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		default:
			return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
		}
	}

	auto ToVk(df::EAddressMode value)->VkSamplerAddressMode {
		switch (value) {
		case df::EAddressMode::Repeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case df::EAddressMode::MirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case df::EAddressMode::ClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case df::EAddressMode::ClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case df::EAddressMode::MirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR;
		default:
			return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
		}
	}
}


vk::ParameterSet::ParameterSet(RenderCore& renderCore, ParameterSetDefinition& definition)
	: m_RenderCore(renderCore)
	, m_Definition(definition) 
	, m_ConstandBufferDataPtr(nullptr) 
	, m_ConstandBufferData(nullptr)
	, m_ActiveDescriptorIndex(-1) 
	, m_UpdateDescriptorSet(true) 
	, m_IsBuilt(false) {
	m_Definition.IncrementRefCount();
}

vk::ParameterSet::~ParameterSet() {
	if (m_Definition.DecrementRefCount() == 0) {
		m_RenderCore.DestroyParameterSetDefinition(&m_Definition);
	}

	for (auto vkDescriptorSet : m_DescriptorSets) {
		m_RenderCore.RemoveDescriptorSet(m_RenderCore.GetVkDescriptorPool(), vkDescriptorSet);
	}

	DFDelete[] m_ConstandBufferData;
}

void vk::ParameterSet::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareFloatParameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareVec2Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareVec3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareVec4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareMat3Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareMat4Parameter(name, defaultValue);
}

void vk::ParameterSet::DeclareTextureParameter(const df::StringView& name) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareTextureParameter(name);
}

void vk::ParameterSet::DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) {
	DFAssert(!m_IsBuilt, "Can't declare parameter - parameter set is already build!");

	m_Definition.DeclareBufferParameter(name, dataType);
}

void vk::ParameterSet::Build() {
	m_Definition.Build();

	m_Buffers.resize(m_Definition.GetBuffers().size());
	m_Textures.resize(m_Definition.GetTextures().size());

	const auto constantBufferSize = m_Definition.GetConstantBufferSize();
	if (constantBufferSize > 0) {
		m_ConstandBufferData = DFNew uint8[constantBufferSize];
		m_ConstandBufferDataPtr = m_ConstandBufferData;

		for (auto c : m_Definition.GetFloatConstants()) {
			auto data = reinterpret_cast<float*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}

		for (auto c : m_Definition.GetVec2Constants()) {
			auto data = reinterpret_cast<Vec2*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}

		for (auto c : m_Definition.GetVec3Constants()) {
			auto data = reinterpret_cast<Vec3*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}

		for (auto c : m_Definition.GetVec4Constants()) {
			auto data = reinterpret_cast<Vec4*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}

		for (auto c : m_Definition.GetMat3Constants()) {
			auto data = reinterpret_cast<Mat3*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}

		for (auto c : m_Definition.GetMat4Constants()) {
			auto data = reinterpret_cast<Mat4*>(m_ConstandBufferDataPtr + c.m_Offset);
			*data = c.m_DefaultValue;
		}
	}

	m_IsBuilt = true;
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

bool vk::ParameterSet::SetFloat(const df::StringView& name, float value) {
	return SetFloatByName(name, value);
}

bool vk::ParameterSet::SetVec2(const df::StringView& name, const Vec2& value) {
	return SetVec2ByName(name, value);
}

bool vk::ParameterSet::SetVec3(const df::StringView& name, const Vec3& value) {
	return SetVec3ByName(name, value);
}

bool vk::ParameterSet::SetVec4(const df::StringView& name, const Vec4& value) {
	return SetVec4ByName(name, value);
}

bool vk::ParameterSet::SetMat3(const df::StringView& name, const Mat3& value) {
	return SetMat3ByName(name, value);
}

bool vk::ParameterSet::SetMat4(const df::StringView& name, const Mat4& value) {
	return SetMat4ByName(name, value);
}

bool vk::ParameterSet::SetTexture(const df::StringView& name, df::Texture* texture) {
	return SetTextureByName(name, static_cast<vk::Texture*>(texture));
}

bool vk::ParameterSet::SetBuffer(const df::StringView& name, df::Buffer* buffer) {
	return SetBufferByName(name, static_cast<vk::Buffer*>(buffer));
}

bool vk::ParameterSet::SetFilter(const df::StringView& textureName, df::EFilter filter) {
	return SetFilterByName(textureName, filter);
}

bool vk::ParameterSet::SetMipMapMode(const df::StringView& textureName, df::EMipmapMode mipMapMode) {
	return SetMipMapModeByName(textureName, mipMapMode);
}

bool vk::ParameterSet::SetAddressMode(const df::StringView& textureName, df::EAddressMode addressMode) {
	return SetAddressModeByName(textureName, addressMode);
}

auto vk::ParameterSet::GetDefinition() const->ParameterSetDefinition& {
	return m_Definition;
}

auto vk::ParameterSet::GetDescriptorSetHandle() const->VkDescriptorSet {
	DFAssert(m_ActiveDescriptorIndex >= 0, "Parameter set should be updated first!");

	return m_DescriptorSets[m_ActiveDescriptorIndex];
}

void vk::ParameterSet::Update() {
	DFAssert(m_IsBuilt, "Can't update parameter set - parameter set is not build yet!");

	if (m_UpdateDescriptorSet) {
		UpdateDescriptorSet();
	}
}

void vk::ParameterSet::SetFloatByOffset(uint32 constOffset, float value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	float* ptr = reinterpret_cast<float*>(m_ConstandBufferDataPtr + constOffset);
	*ptr = value;
}

bool vk::ParameterSet::SetFloatByName(const df::StringView& name, float value) {
	for (auto& c : m_Definition.GetFloatConstants()) {
		if (c.m_Name == name) {
			SetFloatByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetVec2ByOffset(uint32 constOffset, const Vec2& value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	Vec2* ptr = reinterpret_cast<Vec2*>(m_ConstandBufferDataPtr + constOffset);
	*ptr = value;
}

bool vk::ParameterSet::SetVec2ByName(const df::StringView& name, const Vec2& value) {
	for (auto& c : m_Definition.GetVec2Constants()) {
		if (c.m_Name == name) {
			SetVec2ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetVec3ByOffset(uint32 constOffset, const Vec3& value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	Vec3* ptr = reinterpret_cast<Vec3*>(m_ConstandBufferDataPtr + constOffset);
	*ptr = value;
}

bool vk::ParameterSet::SetVec3ByName(const df::StringView& name, const Vec3& value) {
	for (auto& c : m_Definition.GetVec3Constants()) {
		if (c.m_Name == name) {
			SetVec3ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetVec4ByOffset(uint32 constOffset, const Vec4& value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstandBufferDataPtr + constOffset);
	*ptr = value;
}

bool vk::ParameterSet::SetVec4ByName(const df::StringView& name, const Vec4& value) {
	for (auto& c : m_Definition.GetVec4Constants()) {
		if (c.m_Name == name) {
			SetVec4ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetMat3ByOffset(uint32 constOffset, const Mat3& value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	Vec4* ptr = reinterpret_cast<Vec4*>(m_ConstandBufferDataPtr + constOffset);

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

bool vk::ParameterSet::SetMat3ByName(const df::StringView& name, const Mat3& value) {
	for (auto& c : m_Definition.GetMat3Constants()) {
		if (c.m_Name == name) {
			SetMat3ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetMat4ByOffset(uint32 constOffset, const Mat4& value) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	Mat4* ptr = reinterpret_cast<Mat4*>(m_ConstandBufferDataPtr + constOffset);
	*ptr = value;
}

bool vk::ParameterSet::SetMat4ByName(const df::StringView& name, const Mat4& value) {
	for (auto& c : m_Definition.GetMat4Constants()) {
		if (c.m_Name == name) {
			SetMat4ByOffset(c.m_Offset, value);
			return true;
		}
	}
	return false;
}

void vk::ParameterSet::SetBufferByIndex(uint32 index, vk::Buffer* buffer) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	m_Buffers[index] = buffer;
}

bool vk::ParameterSet::SetBufferByName(const df::StringView& name, vk::Buffer* buffer) {
	auto buffers = m_Definition.GetBuffers();

	for (size_t i = 0; i < buffers.size(); ++i) {
		if (buffers[i].m_Name == name) {
			SetBufferByIndex(uint32(i), buffer);
			return true;
		}
	}

	return false;
}

void vk::ParameterSet::SetTextureByIndex(uint32 index, vk::Texture* texture) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	m_Textures[index].m_Texture = texture;
}

bool vk::ParameterSet::SetTextureByName(const df::StringView& name, vk::Texture* texture) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == name) {
			SetTextureByIndex(uint32(i), texture);
			return true;
		}
	}

	return false;
}

void vk::ParameterSet::SetFilterByIndex(uint32 index, df::EFilter filter) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	m_Textures[index].m_CurrentState.m_VkFilter = ToVk(filter);
}

bool vk::ParameterSet::SetFilterByName(const df::StringView& textureName, df::EFilter filter) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == textureName) {
			SetFilterByIndex(uint32(i), filter);
			return true;
		}
	}

	return false;
}

void vk::ParameterSet::SetMipMapModeByIndex(uint32 index, df::EMipmapMode mipMapMode) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	m_Textures[index].m_CurrentState.m_VkMipMapMode = ToVk(mipMapMode);
}

bool vk::ParameterSet::SetMipMapModeByName(const df::StringView& textureName, df::EMipmapMode mipMapMode) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == textureName) {
			SetMipMapModeByIndex(uint32(i), mipMapMode);
			return true;
		}
	}

	return false;
}

void vk::ParameterSet::SetAddressModeByIndex(uint32 index, df::EAddressMode addressMode) {
	DFAssert(m_IsBuilt, "Can't set parameter - parameter set is not build yet!");

	m_Textures[index].m_CurrentState.m_VkAddressMode = ToVk(addressMode);
}

bool vk::ParameterSet::SetAddressModeByName(const df::StringView& textureName, df::EAddressMode addressMode) {
	auto textures = m_Definition.GetTextures();

	for (size_t i = 0; i < textures.size(); ++i) {
		if (textures[i].m_Name == textureName) {
			SetAddressModeByIndex(uint32(i), addressMode);
			return true;
		}
	}

	return false;
}

void vk::ParameterSet::UpdateDescriptorSet() {
	constexpr uint32 MaxDescriptorSetsPerParameterSet = 3;
	m_ActiveDescriptorIndex = (m_ActiveDescriptorIndex + 1) % MaxDescriptorSetsPerParameterSet;

	if (m_DescriptorSets.size() <= m_ActiveDescriptorIndex) {
		m_DescriptorSets.resize(m_ActiveDescriptorIndex + 1);

		m_DescriptorSets[m_ActiveDescriptorIndex] = m_Definition.CreateDescriptorSet();
	}



	m_UpdateDescriptorSet = false;
}