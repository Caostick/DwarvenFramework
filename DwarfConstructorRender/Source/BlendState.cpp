#include <DwarfConstructorRender/BlendState.h>

rf::BlendState::BlendState(EBlendState state) {
	*this = state;
}

rf::BlendState& rf::BlendState::operator = (EBlendState state) {
	switch (state) {
	case rf::EBlendState::None:
		m_BlendEnable = false;
		m_ColorBlendOp = EBlendOp::Add;
		m_AlphaBlendOp = EBlendOp::Add;
		m_SrcColorBlendFactor = EBlendFactor::Zero;
		m_SrcAlphaBlendFactor = EBlendFactor::One;
		m_DstColorBlendFactor = EBlendFactor::Zero;
		m_DstAlphaBlendFactor = EBlendFactor::One;
		break;
	case rf::EBlendState::Alpha:
		m_BlendEnable = true;
		m_ColorBlendOp = rf::EBlendOp::Add;
		m_AlphaBlendOp = rf::EBlendOp::Add;
		m_SrcColorBlendFactor = rf::EBlendFactor::SrcAlpha;
		m_SrcAlphaBlendFactor = rf::EBlendFactor::One;
		m_DstColorBlendFactor = rf::EBlendFactor::OneMinusSrcAlpha;
		m_DstAlphaBlendFactor = rf::EBlendFactor::Zero;
		break;
	default:
		break;
	}

	return *this;
}

bool rf::BlendState::operator==(const BlendState& other) const {
	return
		m_BlendEnable == other.m_BlendEnable &&
		m_ColorBlendOp == other.m_ColorBlendOp &&
		m_AlphaBlendOp == other.m_AlphaBlendOp &&
		m_SrcColorBlendFactor == other.m_SrcColorBlendFactor &&
		m_SrcAlphaBlendFactor == other.m_SrcAlphaBlendFactor &&
		m_DstColorBlendFactor == other.m_DstColorBlendFactor &&
		m_DstAlphaBlendFactor == other.m_DstAlphaBlendFactor;
}

bool rf::BlendState::operator!=(const BlendState& other) const {
	return
		m_BlendEnable != other.m_BlendEnable ||
		m_ColorBlendOp != other.m_ColorBlendOp ||
		m_AlphaBlendOp != other.m_AlphaBlendOp ||
		m_SrcColorBlendFactor != other.m_SrcColorBlendFactor ||
		m_SrcAlphaBlendFactor != other.m_SrcAlphaBlendFactor ||
		m_DstColorBlendFactor != other.m_DstColorBlendFactor ||
		m_DstAlphaBlendFactor != other.m_DstAlphaBlendFactor;
}