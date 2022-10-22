#include <DwarfRender/BlendState.h>

df::BlendState::BlendState(EBlendState state) {
	*this = state;
}

df::BlendState& df::BlendState::operator = (EBlendState state) {
	switch (state) {
	case EBlendState::None:
		m_BlendEnable = false;
		m_ColorBlendOp = EBlendOp::Add;
		m_AlphaBlendOp = EBlendOp::Add;
		m_SrcColorBlendFactor = EBlendFactor::Zero;
		m_SrcAlphaBlendFactor = EBlendFactor::One;
		m_DstColorBlendFactor = EBlendFactor::Zero;
		m_DstAlphaBlendFactor = EBlendFactor::One;
		break;
	case EBlendState::Alpha:
		m_BlendEnable = true;
		m_ColorBlendOp = EBlendOp::Add;
		m_AlphaBlendOp = EBlendOp::Add;
		m_SrcColorBlendFactor = EBlendFactor::SrcAlpha;
		m_SrcAlphaBlendFactor = EBlendFactor::One;
		m_DstColorBlendFactor = EBlendFactor::OneMinusSrcAlpha;
		m_DstAlphaBlendFactor = EBlendFactor::Zero;
		break;
	default:
		break;
	}

	return *this;
}

bool df::BlendState::operator==(const BlendState& other) const {
	return
		m_BlendEnable == other.m_BlendEnable &&
		m_ColorBlendOp == other.m_ColorBlendOp &&
		m_AlphaBlendOp == other.m_AlphaBlendOp &&
		m_SrcColorBlendFactor == other.m_SrcColorBlendFactor &&
		m_SrcAlphaBlendFactor == other.m_SrcAlphaBlendFactor &&
		m_DstColorBlendFactor == other.m_DstColorBlendFactor &&
		m_DstAlphaBlendFactor == other.m_DstAlphaBlendFactor;
}

bool df::BlendState::operator!=(const BlendState& other) const {
	return
		m_BlendEnable != other.m_BlendEnable ||
		m_ColorBlendOp != other.m_ColorBlendOp ||
		m_AlphaBlendOp != other.m_AlphaBlendOp ||
		m_SrcColorBlendFactor != other.m_SrcColorBlendFactor ||
		m_SrcAlphaBlendFactor != other.m_SrcAlphaBlendFactor ||
		m_DstColorBlendFactor != other.m_DstColorBlendFactor ||
		m_DstAlphaBlendFactor != other.m_DstAlphaBlendFactor;
}