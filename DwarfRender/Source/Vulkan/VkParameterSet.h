#pragma once

#include <DwarfRender/ParameterSet.h>

#include <DwarvenCore/Vector.h>

#include "VkSampler.h"

namespace vk {
	class RenderCore;
	class ParameterSetDefinition;
	class Texture;
	class Buffer;
}

namespace vk {
	class ParameterSet : public df::ParameterSet {
	public:
		ParameterSet(RenderCore& renderCore, ParameterSetDefinition& definition);
		virtual ~ParameterSet() override;

		virtual void DeclareFloatParameter(const df::StringView& name, float defaultValue = 0.0f) override;
		virtual void DeclareVec2Parameter(const df::StringView& name, const Vec2& defaultValue = Vec2()) override;
		virtual void DeclareVec3Parameter(const df::StringView& name, const Vec3& defaultValue = Vec3()) override;
		virtual void DeclareVec4Parameter(const df::StringView& name, const Vec4& defaultValue = Vec4()) override;
		virtual void DeclareMat3Parameter(const df::StringView& name, const Mat3& defaultValue = Mat3::Identity()) override;
		virtual void DeclareMat4Parameter(const df::StringView& name, const Mat4& defaultValue = Mat4::Identity()) override;
		virtual void DeclareTextureParameter(const df::StringView& name) override;
		virtual void DeclareBufferParameter(const df::StringView& name, df::EShaderConstantType dataType) override;
		virtual void Build() override;
		virtual auto Clone() const->df::ParameterSet* override;

		virtual bool HasFloatParameter(const df::StringView& name) override;
		virtual bool HasVec2Parameter(const df::StringView& name) override;
		virtual bool HasVec3Parameter(const df::StringView& name) override;
		virtual bool HasVec4Parameter(const df::StringView& name) override;
		virtual bool HasMat3Parameter(const df::StringView& name) override;
		virtual bool HasMat4Parameter(const df::StringView& name) override;
		virtual bool HasTextureParameter(const df::StringView& name) override;
		virtual bool HasBufferParameter(const df::StringView& name) override;

		virtual bool SetFloat(const df::StringView& name, float value) override;
		virtual bool SetVec2(const df::StringView& name, const Vec2& value) override;
		virtual bool SetVec3(const df::StringView& name, const Vec3& value) override;
		virtual bool SetVec4(const df::StringView& name, const Vec4& value) override;
		virtual bool SetMat3(const df::StringView& name, const Mat3& value) override;
		virtual bool SetMat4(const df::StringView& name, const Mat4& value) override;
		virtual bool SetBuffer(const df::StringView& name, df::Buffer* buffer) override;
		virtual bool SetTexture(const df::StringView& name, df::Texture* texture) override;
		virtual bool SetFilter(const df::StringView& textureName, df::EFilter filter) override;
		virtual bool SetMipMapMode(const df::StringView& textureName, df::EMipmapMode mipMapMode) override;
		virtual bool SetAddressMode(const df::StringView& textureName, df::EAddressMode addressMode) override;

	public:
		auto GetDefinition() const->ParameterSetDefinition&;
		auto GetVkDescriptorSet() const->VkDescriptorSet;

		void Update();

	private:
		void SetFloatByOffset(uint32 constOffset, float value);
		bool SetFloatByName(const df::StringView& name, float value);

		void SetVec2ByOffset(uint32 constOffset, const Vec2& value);
		bool SetVec2ByName(const df::StringView& name, const Vec2& value);

		void SetVec3ByOffset(uint32 constOffset, const Vec3& value);
		bool SetVec3ByName(const df::StringView& name, const Vec3& value);

		void SetVec4ByOffset(uint32 constOffset, const Vec4& value);
		bool SetVec4ByName(const df::StringView& name, const Vec4& value);

		void SetMat3ByOffset(uint32 constOffset, const Mat3& value);
		bool SetMat3ByName(const df::StringView& name, const Mat3& value);

		void SetMat4ByOffset(uint32 constOffset, const Mat4& value);
		bool SetMat4ByName(const df::StringView& name, const Mat4& value);

		void SetBufferByIndex(uint32 index, vk::Buffer* buffer);
		bool SetBufferByName(const df::StringView& name, vk::Buffer* buffer);

		void SetTextureByIndex(uint32 index, vk::Texture* texture);
		bool SetTextureByName(const df::StringView& name, vk::Texture* texture);

		void SetFilterByIndex(uint32 index, df::EFilter filter);
		bool SetFilterByName(const df::StringView& textureName, df::EFilter filter);

		void SetMipMapModeByIndex(uint32 index, df::EMipmapMode mipMapMode);
		bool SetMipMapModeByName(const df::StringView& textureName, df::EMipmapMode mipMapMode);

		void SetAddressModeByIndex(uint32 index, df::EAddressMode addressMode);
		bool SetAddressModeByName(const df::StringView& textureName, df::EAddressMode addressMode);

		void UpdateConstantBuffer();
		void UpdateDescriptorSet();

	private:
		struct TextureState {
			uint32 m_Binding = 0;
			vk::Texture* m_Texture = nullptr;
			vk::Sampler* m_Sampler = nullptr;
			vk::SamplerState m_CurrentState;
			VkDescriptorImageInfo m_DescriptorInfo = {};
		};

		struct BufferState {
			uint32 m_Binding = 0;
			vk::Buffer* m_Buffer = nullptr;
			VkDescriptorBufferInfo m_DescriptorInfo = {};
		};

		RenderCore& m_RenderCore;
		ParameterSetDefinition& m_Definition;

		df::Vector<VkWriteDescriptorSet> m_Writes;
		df::Vector<VkDescriptorSet> m_VkDescriptorSets;
		df::Vector<TextureState> m_Textures;
		df::Vector<BufferState> m_Buffers;

		BufferState m_ConstantBuffer;
		uint8* m_ConstandBufferDataPtr;
		uint8* m_ConstandBufferData;

		int32 m_ActiveDescriptorIndex;
		bool m_UpdateConstantBuffer;
		bool m_UpdateDescriptorSet;
		bool m_IsBuilt;
	};
}