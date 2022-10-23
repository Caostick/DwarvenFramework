#include "VkParameterSetDefinition.h"
#include "VkRenderCore.h"
#include "VkDebug.h"

#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Assert.h>

namespace {
	auto GetAlignment(df::EShaderConstantType type)->uint32 {
		constexpr uint32 N = 4;
		switch (type) {
		case df::EShaderConstantType::Float:
			return N;
		case df::EShaderConstantType::Vec2:
			return 2 * N;
		case df::EShaderConstantType::Vec3:
			return 4 * N;
		case df::EShaderConstantType::Vec4:
			return 4 * N;
		case df::EShaderConstantType::Mat3:
			return 4 * N;
		case df::EShaderConstantType::Mat4:
			return 4 * N;
		default:
			return 4 * N;
		}
	}

	auto ShaderConstantTypeToString(df::EShaderConstantType type)->df::String {
		switch (type) {
		case df::EShaderConstantType::Float:
			return "float";
		case df::EShaderConstantType::Vec2:
			return "vec2";
		case df::EShaderConstantType::Vec3:
			return "vec3";
		case df::EShaderConstantType::Vec4:
			return "vec4";
		case df::EShaderConstantType::Mat3:
			return "mat3";
		case df::EShaderConstantType::Mat4:
			return "mat4";
		default:
			return "";
		}
	}
}

vk::ParameterSetDefinition::ParameterSetDefinition(vk::RenderCore& renderCore, const df::StringView& name)
	: m_RenderCore(renderCore) 
	, m_Name(name)
	, m_RefCount(0)
	, m_ConstantBufferSize(0)
	, m_VkDescriptorSetLayout(VK_NULL_HANDLE)
	, m_IsBuilt(false) {
}

vk::ParameterSetDefinition::~ParameterSetDefinition() {
	DFAssert(m_RefCount == 0, "There are still objects which use definition!");

	m_RenderCore.RemoveDescriptorSetLayout(m_VkDescriptorSetLayout);
}

void vk::ParameterSetDefinition::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	vk::ShaderFloatDefinition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_FloatConstants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	vk::ShaderVec2Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec2Constants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	vk::ShaderVec3Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec3Constants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	vk::ShaderVec4Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec4Constants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	vk::ShaderMat3Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Mat3Constants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	vk::ShaderMat4Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Mat4Constants.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareTextureParameter(const df::StringView& name) {
	vk::ShaderTextureDefinition def = {};
	def.m_Name = name;

	m_Textures.emplace_back(def);
}

void vk::ParameterSetDefinition::DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) {
	vk::ShaderBufferDefinition def = {};
	def.m_Name = name;
	def.m_DataType = dataType;

	m_Buffers.emplace_back(def);
}

bool vk::ParameterSetDefinition::HasFloatParameter(const df::StringView& name) {
	for (const auto& param : m_FloatConstants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasVec2Parameter(const df::StringView& name) {
	for (const auto& param : m_Vec2Constants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasVec3Parameter(const df::StringView& name) {
	for (const auto& param : m_Vec3Constants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasVec4Parameter(const df::StringView& name) {
	for (const auto& param : m_Vec4Constants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasMat3Parameter(const df::StringView& name) {
	for (const auto& param : m_Mat3Constants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasMat4Parameter(const df::StringView& name) {
	for (const auto& param : m_Mat4Constants) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasTextureParameter(const df::StringView& name) {
	for (const auto& param : m_Textures) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

bool vk::ParameterSetDefinition::HasBufferParameter(const df::StringView& name) {
	for (const auto& param : m_Buffers) {
		if (param.m_Name == name) {
			return true;
		}
	}

	return false;
}

auto vk::ParameterSetDefinition::GetConstantBufferSize() const->uint32 {
	return m_ConstantBufferSize;
}

auto vk::ParameterSetDefinition::GetConstants() const -> const df::Vector<ShaderConstantDefinition*>& {
	return m_Constants;
}

auto vk::ParameterSetDefinition::GetFloatConstants() const -> const df::Vector<ShaderFloatDefinition>& {
	return m_FloatConstants;
}

auto vk::ParameterSetDefinition::GetVec2Constants() const -> const df::Vector<ShaderVec2Definition>& {
	return m_Vec2Constants;
}

auto vk::ParameterSetDefinition::GetVec3Constants() const -> const df::Vector<ShaderVec3Definition>& {
	return m_Vec3Constants;
}

auto vk::ParameterSetDefinition::GetVec4Constants() const -> const df::Vector<ShaderVec4Definition>& {
	return m_Vec4Constants;
}

auto vk::ParameterSetDefinition::GetMat3Constants() const -> const df::Vector<ShaderMat3Definition>& {
	return m_Mat3Constants;
}

auto vk::ParameterSetDefinition::GetMat4Constants() const -> const df::Vector<ShaderMat4Definition>& {
	return m_Mat4Constants;
}

auto vk::ParameterSetDefinition::GetBuffers() const -> const df::Vector<ShaderBufferDefinition>& {
	return m_Buffers;
}

auto vk::ParameterSetDefinition::GetTextures() const -> const df::Vector<ShaderTextureDefinition>& {
	return m_Textures;
}

auto vk::ParameterSetDefinition::GetName() const -> const df::String& {
	return m_Name;
}

auto vk::ParameterSetDefinition::MakeShaderSnippet(uint32 idx) const->df::String {
	df::String snippet;

	snippet.reserve(1024);

	if (!m_Constants.empty()) {
		snippet += df::String("layout(set = ");
		snippet += std::to_string(idx);
		snippet += ", binding = ";
		snippet += std::to_string(0);
		snippet += ") uniform _";
		snippet += m_Name;
		snippet += "CB";
		snippet += " {";
		snippet += "\n";

		for (const auto& constant : m_Constants) {
			snippet += "    " + ShaderConstantTypeToString(constant->GetType()) + " ";
			snippet += constant->m_Name;
			snippet += ";";
			snippet += "\n";
		}

		snippet += "} ";
		snippet += m_Name;
		snippet += "CB;";
		snippet += "\n";
		snippet += "\n";
	}

	if (!m_Buffers.empty()) {
		for (const auto& buffer : m_Buffers) {
			snippet += "layout(set = ";
			snippet += std::to_string(idx);
			snippet += ", binding = ";
			snippet += std::to_string(buffer.m_Binding);
			snippet += ") readonly buffer _";
			snippet += buffer.m_Name;
			snippet += " {\n";
			snippet += "    ";
			snippet += ShaderConstantTypeToString(buffer.m_DataType);
			snippet += " Data[];\n";
			snippet += "} ";
			snippet += buffer.m_Name;
			snippet += ";";

			snippet += "\n";
		}
	}

	if (!m_Textures.empty()) {
		for (const auto& texture : m_Textures) {
			snippet += "layout(set = ";
			snippet += std::to_string(idx);
			snippet += ", binding = ";
			snippet += std::to_string(texture.m_Binding);
			snippet += ") uniform sampler2D ";
			snippet += texture.m_Name;
			snippet += ";";

			snippet += "\n";
		}
	}

	snippet.shrink_to_fit();


	return snippet;
}

auto vk::ParameterSetDefinition::CreateDescriptorSet()->VkDescriptorSet {
	VkDevice vkDevice = m_RenderCore.GetVkDevice();
	VkDescriptorPool vkDescriptorPool = m_RenderCore.GetVkDescriptorPool();

	VkDescriptorSetAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool = vkDescriptorPool;
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &m_VkDescriptorSetLayout;

	VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;
	vk::API::AllocateDescriptorSets(vkDevice, &allocateInfo, &vkDescriptorSet);

	DFVkDebugName(vkDevice, vkDescriptorSet, m_Name);

	return vkDescriptorSet;
}

auto vk::ParameterSetDefinition::GetVkDescriptorSetLayout() const->VkDescriptorSetLayout {
	return m_VkDescriptorSetLayout;
}

auto vk::ParameterSetDefinition::IncrementRefCount()->uint32 {
	return ++m_RefCount;
}

auto vk::ParameterSetDefinition::DecrementRefCount() ->uint32 {
	DFAssert(m_RefCount != 0, "Invalid parameter set definition ref count!");
	return --m_RefCount;
}

void vk::ParameterSetDefinition::Build() {
	if (m_IsBuilt) {
		return;
	}

	for (auto& constant : m_Mat4Constants) {
		m_Constants.push_back(&constant);
	}
	for (auto& constant : m_Vec4Constants) {
		m_Constants.push_back(&constant);
	}
	for (auto& constant : m_Vec2Constants) {
		m_Constants.push_back(&constant);
	}
	for (auto& constant : m_Vec3Constants) {
		m_Constants.push_back(&constant);
	}
	for (auto& constant : m_FloatConstants) {
		m_Constants.push_back(&constant);
	}
	for (auto& constant : m_Mat3Constants) {
		m_Constants.push_back(&constant);
	}

	m_ConstantBufferSize = 0;
	for (auto& constant : m_Constants) {
		const uint32 alignment = GetAlignment(constant->GetType());
		const uint32 padding = m_ConstantBufferSize % alignment;

		constant->m_Offset = m_ConstantBufferSize + padding;
		m_ConstantBufferSize += constant->GetSize() + padding;
	}

	uint32 bindingIndex = 1;

	for (auto& buffer : m_Buffers) {
		buffer.m_Binding = bindingIndex++;
	}

	for (auto& texture : m_Textures) {
		texture.m_Binding = bindingIndex++;
	}

	CreateRenderData();

	m_IsBuilt = true;
}

void vk::ParameterSetDefinition::CreateRenderData() {
	VkDevice vkDevice = m_RenderCore.GetVkDevice();

	const bool hasConstantBuffer = (m_ConstantBufferSize != 0);
	const uint32 bufferCount = uint32(m_Buffers.size());
	const uint32 textureCount = uint32(m_Textures.size());

	const uint32 descriptorCount =
		(hasConstantBuffer ? 1 : 0) +
		textureCount + bufferCount;

	df::Vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings(descriptorCount);
	
	if (hasConstantBuffer) {
		const uint32 binding = 0;

		descriptorSetLayoutBindings[binding] = {};
		descriptorSetLayoutBindings[binding].binding = binding;
		descriptorSetLayoutBindings[binding].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetLayoutBindings[binding].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptorSetLayoutBindings[binding].descriptorCount = 1;
	}

	for (uint32 i = 0; i < bufferCount; ++i) {
		const auto& buffer = m_Buffers[i];
		const uint32 binding = buffer.m_Binding;

		descriptorSetLayoutBindings[binding] = {};
		descriptorSetLayoutBindings[binding].binding = binding;
		descriptorSetLayoutBindings[binding].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSetLayoutBindings[binding].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptorSetLayoutBindings[binding].descriptorCount = 1;
	}

	for (uint32 i = 0; i < textureCount; ++i) {
		const auto& texture = m_Textures[i];
		const uint32 binding = texture.m_Binding;

		descriptorSetLayoutBindings[binding] = {};
		descriptorSetLayoutBindings[binding].binding = binding;
		descriptorSetLayoutBindings[binding].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorSetLayoutBindings[binding].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptorSetLayoutBindings[binding].descriptorCount = 1;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = descriptorCount;
	layoutInfo.pBindings = descriptorSetLayoutBindings.data();

	if (vk::API::CreateDescriptorSetLayout(vkDevice, &layoutInfo, vk::Allocator(), &m_VkDescriptorSetLayout) != VK_SUCCESS) {
		DFAssert(false, "Can't create DescriptorSetLayout!");
	}

	DFVkDebugName(vkDevice, m_VkDescriptorSetLayout, m_Name);
}