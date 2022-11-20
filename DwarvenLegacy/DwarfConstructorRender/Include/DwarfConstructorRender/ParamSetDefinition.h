#pragma once

#include <DwarfConstructorRender/Types.h>
#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/TypeList.h>

#include <DwarvenCore/Math/Math.h>

namespace rf {
	class RenderCore;
}

namespace rf {
	struct ShaderConstantDefinition {
		df::String m_Name;
		uint32 m_Offset;

		virtual auto GetType() const->rf::EShaderParameterType = 0;
		virtual auto GetSize() const->uint32 = 0;
	};

	template<typename T, rf::EShaderParameterType ConstType>
	struct TShaderConstantDefinition : public ShaderConstantDefinition {
		T m_DefaultValue;

		auto GetType() const->rf::EShaderParameterType override;
		auto GetSize() const->uint32 override;
	};

	using ShaderFloatDefinition = TShaderConstantDefinition<float, rf::EShaderParameterType::Float>;
	using ShaderVec2Definition = TShaderConstantDefinition<Vec2, rf::EShaderParameterType::Vec2>;
	using ShaderVec3Definition = TShaderConstantDefinition<Vec3, rf::EShaderParameterType::Vec3>;
	using ShaderVec4Definition = TShaderConstantDefinition<Vec4, rf::EShaderParameterType::Vec4>;
	using ShaderMat3Definition = TShaderConstantDefinition<Mat3, rf::EShaderParameterType::Mat3>;
	using ShaderMat4Definition = TShaderConstantDefinition<Mat4, rf::EShaderParameterType::Mat4>;

	struct ShaderTextureDefinition {
		uint32 m_Binding;
		df::String m_Name;
		df::String m_DefaultTextureName;
	};

	struct ShaderBufferDefinition {
		uint32 m_Binding;
		rf::EShaderParameterType m_DataType;
		bool m_Multiframed;
		df::String m_Name;
	};

	class ParamSetDefinition {
	public:
		ParamSetDefinition(const df::StringView& name, uint32 index);

		void DeclareFloatParameter(const df::StringView& name, float defaultValue);
		void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue);
		void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue);
		void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue);
		void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue);
		void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue);
		void DeclareTextureParameter(const df::StringView& name, const df::StringView& defaultTextureName = "");
		void DeclareBufferParameter(const df::StringView& name, rf::EShaderParameterType dataType, bool multiframed = false);

		void Build();

		void CreateRenderData(RenderCore& renderCore);
		void DestroyRenderData(RenderCore& renderCore);

		auto GetName() const -> const df::String&;
		auto GetIndex() const->uint32;
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
		auto GetDescriptorSetLayout() const->rf::DescriptorSetLayoutId;
		auto CreateDescriptorSet(RenderCore& renderCore) const->rf::DescriptorSetId;

		auto GetShaderSnippet() const -> const df::String&;

	private:
		void MakeShaderSnippet();

		df::String m_Name;
		df::String m_Snippet;
		uint32 m_Index;

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

		rf::DescriptorSetLayoutId m_DescriptorSetLayout;
	};

	template<typename TL, typename Enable = void>
	struct TParamSetDeclarator;

	template<typename TL>
	struct TParamSetDeclarator<TL, typename std::enable_if<df::Length<TL>::value == 0>::type> {
		static void Declare(ParamSetDefinition&);
	};

	template<typename TL>
	struct TParamSetDeclarator<TL, typename std::enable_if<df::Length<TL>::value != 0>::type> {
	public:
		static void Declare(ParamSetDefinition& params);
	};
}

#include <DwarfConstructorRender/ParamSetDefinition.inl>