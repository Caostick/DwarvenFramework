#pragma once

#include "VkAPI.h"

#include <DwarfRender/ParameterSet.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

namespace vk {
	class RenderCore;
}

namespace vk {
	struct ShaderConstantDefinition {
		df::String m_Name;
		uint32 m_Offset = 0;

		virtual auto GetType() const->df::EShaderConstantType = 0;
		virtual auto GetSize() const->uint32 = 0;
	};

	template<typename T, df::EShaderConstantType ConstType>
	struct TShaderConstantDefinition : public ShaderConstantDefinition {
		T m_DefaultValue;

		auto GetType() const->df::EShaderConstantType override;
		auto GetSize() const->uint32 override;
	};

	using ShaderFloatDefinition = TShaderConstantDefinition<float, df::EShaderConstantType::Float>;
	using ShaderVec2Definition = TShaderConstantDefinition<Vec2, df::EShaderConstantType::Vec2>;
	using ShaderVec3Definition = TShaderConstantDefinition<Vec3, df::EShaderConstantType::Vec3>;
	using ShaderVec4Definition = TShaderConstantDefinition<Vec4, df::EShaderConstantType::Vec4>;
	using ShaderMat3Definition = TShaderConstantDefinition<Mat3, df::EShaderConstantType::Mat3>;
	using ShaderMat4Definition = TShaderConstantDefinition<Mat4, df::EShaderConstantType::Mat4>;

	struct ShaderTextureDefinition {
		uint32 m_Binding;
		df::String m_Name;
	};

	struct ShaderBufferDefinition {
		uint32 m_Binding;
		df::EShaderConstantType m_DataType;
		df::String m_Name;
	};

	class ParameterSetDefinition {
	public:
		ParameterSetDefinition(vk::RenderCore& renderCore, const df::StringView& name);
		~ParameterSetDefinition();

		void DeclareFloatParameter(const df::StringView& name, float defaultValue);
		void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue);
		void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue);
		void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue);
		void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue);
		void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue);
		void DeclareTextureParameter(const df::StringView& name);
		void DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType);

		bool HasFloatParameter(const df::StringView& name);
		bool HasVec2Parameter(const df::StringView& name);
		bool HasVec3Parameter(const df::StringView& name);
		bool HasVec4Parameter(const df::StringView& name);
		bool HasMat3Parameter(const df::StringView& name);
		bool HasMat4Parameter(const df::StringView& name);
		bool HasTextureParameter(const df::StringView& name);
		bool HasBufferParameter(const df::StringView& name);

		auto GetConstantBufferSize() const->uint32;
		auto GetConstants() const -> const df::Vector<ShaderConstantDefinition*>&;
		auto GetFloatConstants() const -> const df::Vector<ShaderFloatDefinition>&;
		auto GetVec2Constants() const -> const df::Vector<ShaderVec2Definition>&;
		auto GetVec3Constants() const -> const df::Vector<ShaderVec3Definition>&;
		auto GetVec4Constants() const -> const df::Vector<ShaderVec4Definition>&;
		auto GetMat3Constants() const -> const df::Vector<ShaderMat3Definition>&;
		auto GetMat4Constants() const -> const df::Vector<ShaderMat4Definition>&;
		auto GetBuffers() const -> const df::Vector<ShaderBufferDefinition>&;
		auto GetTextures() const -> const df::Vector<ShaderTextureDefinition>&;

	public:
		auto GetName() const -> const df::String&;
		auto MakeShaderSnippet(uint32 idx) const->df::String;

		auto IncrementRefCount()->uint32;
		auto DecrementRefCount()->uint32;

	private:
		void Build();
		void CreateRenderData();

		vk::RenderCore& m_RenderCore;

		df::String m_Name;

		uint32 m_RefCount;
		uint32 m_ConstantBufferSize;

		df::Vector<ShaderConstantDefinition*> m_Constants;
		df::Vector<ShaderFloatDefinition> m_FloatConstants;
		df::Vector<ShaderVec2Definition> m_Vec2Constants;
		df::Vector<ShaderVec3Definition> m_Vec3Constants;
		df::Vector<ShaderVec4Definition> m_Vec4Constants;
		df::Vector<ShaderMat3Definition> m_Mat3Constants;
		df::Vector<ShaderMat4Definition> m_Mat4Constants;

		df::Vector<ShaderBufferDefinition> m_Buffers;
		df::Vector<ShaderTextureDefinition> m_Textures;

		VkDescriptorSetLayout m_VkDescriptorSetLayout;
	};
}

#include "VkParameterSetDefinition.inl"