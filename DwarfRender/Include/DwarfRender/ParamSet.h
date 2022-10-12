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

		void SetFloat(uint32 constOffset, float value);
		bool SetFloat(const df::StringView& name, float value);
		auto GetFloat(uint32 constOffset) const -> float;
		auto GetFloat(const df::StringView& name) const -> float;

		void SetVec2(uint32 constOffset, const Vec2& value);
		bool SetVec2(const df::StringView& name, const Vec2& value);
		auto GetVec2(uint32 constOffset) const -> const Vec2&;
		auto GetVec2(const df::StringView& name) const -> Vec2;

		void SetVec3(uint32 constOffset, const Vec3& value);
		bool SetVec3(const df::StringView& name, const Vec3& value);
		auto GetVec3(uint32 constOffset) const -> const Vec3&;
		auto GetVec3(const df::StringView& name) const -> Vec3;

		void SetVec4(uint32 constOffset, const Vec4& value);
		bool SetVec4(const df::StringView& name, const Vec4& value);
		auto GetVec4(uint32 constOffset) const -> const Vec4&;
		auto GetVec4(const df::StringView& name) const -> Vec4;

		void SetMat3(uint32 constOffset, const Mat3& value);
		bool SetMat3(const df::StringView& name, const Mat3& value);
		auto GetMat3(uint32 constOffset) const -> Mat3;
		auto GetMat3(const df::StringView& name) const->Mat3;

		void SetMat4(uint32 constOffset, const Mat4& value);
		bool SetMat4(const df::StringView& name, const Mat4& value);
		auto GetMat4(uint32 constOffset) const -> const Mat4&;
		auto GetMat4(const df::StringView& name) const -> Mat4;

		void SetBuffer(uint32 index, rf::BufferId buffer, bool isHistory = false);
		bool SetBuffer(const df::StringView& name, rf::BufferId buffer, bool isHistory = false);
		auto GetBuffer(uint32 index, bool isHistory = false) const->rf::BufferId;
		auto GetBuffer(const df::StringView& name, bool isHistory = false) const->rf::BufferId;

		void SetTexture(uint32 index, rf::TextureId texture);
		bool SetTexture(const df::StringView& name, rf::TextureId texture);
		auto GetTexture(uint32 index) const->rf::TextureId;
		auto GetTexture(const df::StringView& name) const->rf::TextureId;

		void SetSampler(uint32 index, rf::SamplerId sampler);
		bool SetSampler(const df::StringView& name, rf::SamplerId sampler);
		auto GetSampler(uint32 index) const->rf::SamplerId;
		auto GetSampler(const df::StringView& name) const->rf::SamplerId;

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