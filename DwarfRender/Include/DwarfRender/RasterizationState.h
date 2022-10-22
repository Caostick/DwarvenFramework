#pragma once

namespace df {
	enum class ERasterizationState {
		Fill,
		Line,
		Point,

		COUNT
	};

	enum class EPolygonMode {
		Fill,
		Line,
		Point,

		COUNT
	};

	enum class EFrontFace {
		CounterClockwise,
		Clockwise,

		COUNT
	};

	enum class ECullMode {
		None,
		Front,
		Back,
		FrontAndBack,

		COUNT
	};

	struct RasterizationState {
		RasterizationState() = default;
		RasterizationState(RasterizationState&&) = default;
		RasterizationState(const RasterizationState&) = default;
		RasterizationState(ERasterizationState state);
		RasterizationState& operator = (RasterizationState&&) = default;
		RasterizationState& operator = (const RasterizationState&) = default;
		RasterizationState& operator = (ERasterizationState state);

		bool operator == (const RasterizationState& other) const;
		bool operator != (const RasterizationState& other) const;

		bool m_RasterizerDiscardEnable = false;

		EPolygonMode m_PolygonMode = EPolygonMode::Fill;
		EFrontFace m_FrontFace = EFrontFace::CounterClockwise;
		ECullMode m_CullMode = ECullMode::Back;
	};
}