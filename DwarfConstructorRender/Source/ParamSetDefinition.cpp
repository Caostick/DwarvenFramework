#include <DwarfConstructorRender/ParamSetDefinition.h>

#include "RenderCore.h"

#include <DwarvenCore/DebugName.h>

namespace {
	auto GetAlignment(rf::EShaderParameterType type)->uint32 {
		constexpr uint32 N = 4;
		switch (type) {
		case rf::EShaderParameterType::Float:
			return N;
		case rf::EShaderParameterType::Vec2:
			return 2 * N;
		case rf::EShaderParameterType::Vec3:
			return 4 * N;
		case rf::EShaderParameterType::Vec4:
			return 4 * N;
		case rf::EShaderParameterType::Mat3:
			return 4 * N;
		case rf::EShaderParameterType::Mat4:
			return 4 * N;
		default:
			return 4 * N;
		}
	}

	auto ShaderParameterTypeToString(rf::EShaderParameterType type)->df::String {
		switch (type) {
		case rf::EShaderParameterType::Float:
			return "float";
		case rf::EShaderParameterType::Vec2:
			return "vec2";
		case rf::EShaderParameterType::Vec3:
			return "vec3";
		case rf::EShaderParameterType::Vec4:
			return "vec4";
		case rf::EShaderParameterType::Mat3:
			return "mat3";
		case rf::EShaderParameterType::Mat4:
			return "mat4";
		default:
			return "";
		}
	}
}

rf::ParamSetDefinition::ParamSetDefinition(const df::StringView& name, uint32 index)
	: m_Name(name)
	, m_Index(index) 
	, m_ConstantBufferSize(0)
	, m_DescriptorSetLayout(nullptr)
{}

void rf::ParamSetDefinition::DeclareFloatParameter(const df::StringView& name, float defaultValue) {
	rf::ShaderFloatDefinition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_FloatConstants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue) {
	rf::ShaderVec2Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec2Constants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue) {
	rf::ShaderVec3Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec3Constants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue) {
	rf::ShaderVec4Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Vec4Constants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue) {
	rf::ShaderMat3Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Mat3Constants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue) {
	rf::ShaderMat4Definition def = {};
	def.m_Name = name;
	def.m_DefaultValue = defaultValue;

	m_Mat4Constants.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareTextureParameter(const df::StringView& name, const df::StringView& defaultTextureName /*= ""*/) {
	rf::ShaderTextureDefinition def = {};
	def.m_Name = name;
	def.m_DefaultTextureName = defaultTextureName;

	m_Textures.emplace_back(def);
}

void rf::ParamSetDefinition::DeclareBufferParameter(const df::StringView& name, rf::EShaderParameterType dataType, bool multiframed /*= false*/) {
	rf::ShaderBufferDefinition def = {};
	def.m_Name = name;
	def.m_DataType = dataType;
	def.m_Multiframed = multiframed;

	m_Buffers.emplace_back(def);
}

void rf::ParamSetDefinition::Build() {
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

	MakeShaderSnippet();
}

void rf::ParamSetDefinition::CreateRenderData(RenderCore& renderCore) {
	const bool hasConstantBuffer = (m_ConstantBufferSize != 0);
	const uint32 bufferCount = uint32(m_Buffers.size());
	const uint32 textureCount = uint32(m_Textures.size());

	const uint32 descriptorCount =
		(hasConstantBuffer ? 1 : 0) +
		textureCount + bufferCount;

	df::Vector<rf::DescriptorDefinition> descriptors(descriptorCount);
	if (hasConstantBuffer) {
		const uint32 binding = 0;

		descriptors[binding].m_BindingIndex = binding;
		descriptors[binding].m_Type = EDescriptorType::UniformBuffer;
		descriptors[binding].m_ShaderStageFlags = rf::EShaderStage::Vertex | rf::EShaderStage::Fragment;
		descriptors[binding].m_Count = 2;
	}

	for (uint32 i = 0; i < bufferCount; ++i) {
		const auto& buffer = m_Buffers[i];
		const uint32 binding = buffer.m_Binding;

		descriptors[binding].m_BindingIndex = binding;
		descriptors[binding].m_Type = EDescriptorType::StorageBuffer;
		descriptors[binding].m_ShaderStageFlags = rf::EShaderStage::Vertex | rf::EShaderStage::Fragment;
		descriptors[binding].m_Count = buffer.m_Multiframed ? 2 : 1;
	}

	for (uint32 i = 0; i < textureCount; ++i) {
		const auto& texture = m_Textures[i];
		const uint32 binding = texture.m_Binding;

		descriptors[binding].m_BindingIndex = binding;
		descriptors[binding].m_Type = EDescriptorType::CombinedImageSampler;
		descriptors[binding].m_ShaderStageFlags = rf::EShaderStage::Vertex | rf::EShaderStage::Fragment;
		descriptors[binding].m_Count = 1;
	}

	m_DescriptorSetLayout = renderCore.CreateDescriptorSetLayout(df::DebugName("%s Descriptor Set Layout", m_Name.c_str()), descriptors);
}

void rf::ParamSetDefinition::DestroyRenderData(RenderCore& renderCore) {
	renderCore.DestroyDescriptorSetLayout(m_DescriptorSetLayout);
}

auto rf::ParamSetDefinition::GetName() const -> const df::String& {
	return m_Name;
}

auto rf::ParamSetDefinition::GetIndex() const->uint32 {
	return m_Index;
}

auto rf::ParamSetDefinition::GetConstantBufferSize() const->uint32 {
	return m_ConstantBufferSize;
}

auto rf::ParamSetDefinition::GetConstants() const -> const df::Vector<ShaderConstantDefinition*>& {
	return m_Constants;
}

auto rf::ParamSetDefinition::GetFloatConstants() const -> const df::Vector<ShaderFloatDefinition>& {
	return m_FloatConstants;
}

auto rf::ParamSetDefinition::GetVec2Constants() const -> const df::Vector<ShaderVec2Definition>& {
	return m_Vec2Constants;
}

auto rf::ParamSetDefinition::GetVec3Constants() const -> const df::Vector<ShaderVec3Definition>& {
	return m_Vec3Constants;
}

auto rf::ParamSetDefinition::GetVec4Constants() const -> const df::Vector<ShaderVec4Definition>& {
	return m_Vec4Constants;
}

auto rf::ParamSetDefinition::GetMat3Constants() const -> const df::Vector<ShaderMat3Definition>& {
	return m_Mat3Constants;
}

auto rf::ParamSetDefinition::GetMat4Constants() const -> const df::Vector<ShaderMat4Definition>& {
	return m_Mat4Constants;
}

auto rf::ParamSetDefinition::GetBuffers() const -> const df::Vector<ShaderBufferDefinition>& {
	return m_Buffers;
}

auto rf::ParamSetDefinition::GetTextures() const -> const df::Vector<ShaderTextureDefinition>& {
	return m_Textures;
}

auto rf::ParamSetDefinition::GetDescriptorSetLayout() const->rf::DescriptorSetLayoutId {
	return m_DescriptorSetLayout;
}

auto rf::ParamSetDefinition::CreateDescriptorSet(RenderCore& renderCore) const->rf::DescriptorSetId {
	return renderCore.CreateDescriptorSet(df::DebugName("%s Descriptor Set", m_Name.c_str()), m_Index, m_DescriptorSetLayout);
}

auto rf::ParamSetDefinition::GetShaderSnippet() const -> const df::String& {
	return m_Snippet;
}

void rf::ParamSetDefinition::MakeShaderSnippet() {
	m_Snippet.clear();
	m_Snippet.reserve(1024);

	m_Snippet += "\n";

	if (!m_Constants.empty()) {
		m_Snippet += df::String("layout(set = ");
		m_Snippet += std::to_string(m_Index);
		m_Snippet += ", binding = ";
		m_Snippet += std::to_string(0);
		m_Snippet += ") uniform _";
		m_Snippet += m_Name;
		m_Snippet += "UBO";
		m_Snippet += " {";
		m_Snippet += "\n";

		for (const auto& constant : m_Constants) {
			m_Snippet += "  " + ShaderParameterTypeToString(constant->GetType()) + " ";
			m_Snippet += constant->m_Name;
			m_Snippet += ";";
			m_Snippet += "\n";
		}

		m_Snippet += "} ";
		m_Snippet += m_Name;
		m_Snippet += "UBO[2];";
		m_Snippet += "\n";
		m_Snippet += "\n";
	}

	if (!m_Buffers.empty()) {
		for (const auto& buffer : m_Buffers) {
			m_Snippet += "layout(set = ";
			m_Snippet += std::to_string(m_Index);
			m_Snippet += ", binding = ";
			m_Snippet += std::to_string(buffer.m_Binding);
			m_Snippet += ") readonly buffer _";
			m_Snippet += buffer.m_Name;
			m_Snippet += " {\n";
			m_Snippet += "  ";
			m_Snippet += ShaderParameterTypeToString(buffer.m_DataType);
			m_Snippet += " data[];\n";
			m_Snippet += "} ";
			m_Snippet += buffer.m_Name;
			if (buffer.m_Multiframed) {
				m_Snippet += "[2]";
			}
			m_Snippet += ";";

			m_Snippet += "\n";
		}
	}

	if (!m_Textures.empty()) {
		for (const auto& texture : m_Textures) {
			m_Snippet += "layout(set = ";
			m_Snippet += std::to_string(m_Index);
			m_Snippet += ", binding = ";
			m_Snippet += std::to_string(texture.m_Binding);
			m_Snippet += ") uniform sampler2D ";
			m_Snippet += texture.m_Name;
			m_Snippet += ";";

			m_Snippet += "\n";
		}
	}

	m_Snippet.shrink_to_fit();
}