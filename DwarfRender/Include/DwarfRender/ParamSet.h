#pragma once

#include <DwarfRender/ObjectId.h>
#include <DwarfRender/ShaderParameter.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Math/Math.h>

namespace rf {
	class ParamSetDefinition;
	class RenderCore;
}

namespace rf {
	class ParamSet {
	public:
		ParamSet(const ParamSetDefinition& definition);

		void Init(rf::RenderCore& renderCore, bool isStatic);
		void Release(rf::RenderCore& renderCore);

		bool Update(uint32 frameIndex);

		void SetFloatByOffset(uint32 constOffset, float value);
		bool SetFloatByName(const df::StringView& name, float value);
		auto GetFloatByOffset(uint32 constOffset) const -> float;
		auto GetFloatByName(const df::StringView& name) const -> float;

		void SetVec2ByOffset(uint32 constOffset, const Vec2& value);
		bool SetVec2ByName(const df::StringView& name, const Vec2& value);
		auto GetVec2ByOffset(uint32 constOffset) const -> const Vec2&;
		auto GetVec2ByName(const df::StringView& name) const -> Vec2;

		void SetVec3ByOffset(uint32 constOffset, const Vec3& value);
		bool SetVec3ByName(const df::StringView& name, const Vec3& value);
		auto GetVec3ByOffset(uint32 constOffset) const -> const Vec3&;
		auto GetVec3ByName(const df::StringView& name) const -> Vec3;

		void SetVec4ByOffset(uint32 constOffset, const Vec4& value);
		bool SetVec4ByName(const df::StringView& name, const Vec4& value);
		auto GetVec4ByOffset(uint32 constOffset) const -> const Vec4&;
		auto GetVec4ByName(const df::StringView& name) const -> Vec4;

		void SetMat3ByOffset(uint32 constOffset, const Mat3& value);
		bool SetMat3ByName(const df::StringView& name, const Mat3& value);
		auto GetMat3ByOffset(uint32 constOffset) const -> Mat3;
		auto GetMat3ByName(const df::StringView& name) const->Mat3;

		void SetMat4ByOffset(uint32 constOffset, const Mat4& value);
		bool SetMat4ByName(const df::StringView& name, const Mat4& value);
		auto GetMat4ByOffset(uint32 constOffset) const -> const Mat4&;
		auto GetMat4ByName(const df::StringView& name) const -> Mat4;

		void SetBufferByIndex(uint32 index, rf::BufferId buffer, bool isHistory = false);
		bool SetBufferByName(const df::StringView& name, rf::BufferId buffer, bool isHistory = false);
		auto GetBufferByIndex(uint32 index, bool isHistory = false) const->rf::BufferId;
		auto GetBufferByName(const df::StringView& name, bool isHistory = false) const->rf::BufferId;

		void SetTextureByIndex(uint32 index, rf::TextureId texture);
		bool SetTextureByName(const df::StringView& name, rf::TextureId texture);
		auto GetTextureByIndex(uint32 index) const->rf::TextureId;
		auto GetTextureByName(const df::StringView& name) const->rf::TextureId;

		void SetSamplerByIndex(uint32 index, rf::SamplerId sampler);
		bool SetSamplerByName(const df::StringView& name, rf::SamplerId sampler);
		auto GetSamplerByIndex(uint32 index) const->rf::SamplerId;
		auto GetSamplerByName(const df::StringView& name) const->rf::SamplerId;

	private:
		void InitDefaultValues();

		const ParamSetDefinition& m_Definition;

		bool m_IsStatic;
		int32 m_PrevFrameIndex;

		df::Vector<DescriptorSetId> m_DescriptorSet;
		df::Vector<TextureId> m_Textures;
		df::Vector<SamplerId> m_Samplers;
		df::Vector<BufferId> m_Buffers;
		df::Vector<BufferId> m_SecondaryBuffers;

		BufferId m_ConstantBuffer;
		uint8* m_ConstandBufferData;
		uint8* m_ConstandBufferDataPtr;
	};

	template<typename ParamList>
	class TParamSet : public ParamSet {
	public:
		TParamSet(const ParamSetDefinition& definition);

		template<typename ParamType, typename DataType = typename ParamType::DataType>
		void SetConstant(const DataType& value);

		template<typename ParamType, typename DataType = typename ParamType::DataType>
		auto GetConstant() const->DataType;

		template<typename ParamType>
		void SetBuffer(rf::BufferId buffer, bool isHistory = false);

		template<typename ParamType>
		auto GetBuffer(bool isHistory = false) const ->rf::BufferId;

		template<typename ParamType>
		void SetTexture(rf::TextureId texture);

		template<typename ParamType>
		auto GetTexture() const->rf::TextureId;

		template<typename ParamType>
		void SetSampler(rf::SamplerId sampler);

		template<typename ParamType>
		auto GetSampler() const->rf::SamplerId;
	};
}

#include <DwarfRender/ParamSet.inl>