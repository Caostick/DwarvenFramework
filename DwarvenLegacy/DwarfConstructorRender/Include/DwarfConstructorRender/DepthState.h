#pragma once

namespace rf {
	enum class EDepthStencilState {
		Never,
		Equal,
		EqualNoWrite,
		GreaterEqual,
		GreaterEqualNoWrite,
		LessEqual,
		LessEqualNoWrite,
		Always,
		AlwaysNoWrite,

		COUNT
	};

	enum class EDepthCompareOp {
		Never,
		Equal,
		NotEqual,
		Greater,
		GreaterEqual,
		Less,
		LessEqual,
		Always,

		COUNT
	};

	enum class EStencilOp {
		Keep,
		Zero,
		Replace,
		Increment,
		Decrement,
		Invert,

		COUNT
	};

	struct DepthState {
		DepthState() = default;
		DepthState(DepthState&&) = default;
		DepthState(const DepthState&) = default;
		DepthState(EDepthStencilState state);
		DepthState& operator = (DepthState&&) = default;
		DepthState& operator = (const DepthState&) = default;
		DepthState& operator = (EDepthStencilState state);

		bool operator == (const DepthState& other) const;
		bool operator != (const DepthState& other) const;

		bool m_DepthTestEnable = false;
		bool m_DepthWriteEnable = false;
		bool m_StencilTestEnable = false;

		EDepthCompareOp m_DepthCompareOp = EDepthCompareOp::LessEqual;
		EStencilOp m_StencilOp = EStencilOp::Keep;
	};
}