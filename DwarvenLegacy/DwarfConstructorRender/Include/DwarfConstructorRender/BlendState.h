#pragma once

namespace rf {
	enum class EBlendState {
		None,
		Alpha,

		COUNT
	};

	enum class EBlendOp {
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,

		COUNT
	};

	enum class EBlendFactor {
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,

		COUNT
	};

	struct BlendState {
		BlendState() = default;
		BlendState(BlendState&&) = default;
		BlendState(const BlendState&) = default;
		BlendState(EBlendState state);
		BlendState& operator = (BlendState&&) = default;
		BlendState& operator = (const BlendState&) = default;
		BlendState& operator = (EBlendState state);

		bool operator == (const BlendState& other) const;
		bool operator != (const BlendState& other) const;

		bool m_BlendEnable = false;

		EBlendOp m_ColorBlendOp = EBlendOp::Add;
		EBlendOp m_AlphaBlendOp = EBlendOp::Add;
		EBlendFactor m_SrcColorBlendFactor = EBlendFactor::Zero;
		EBlendFactor m_SrcAlphaBlendFactor = EBlendFactor::One;
		EBlendFactor m_DstColorBlendFactor = EBlendFactor::Zero;
		EBlendFactor m_DstAlphaBlendFactor = EBlendFactor::One;
	};
}