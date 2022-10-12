#include <DwarfRender/DepthState.h>

rf::DepthState::DepthState(EDepthStencilState state) {
	*this = state;
}

rf::DepthState& rf::DepthState::operator = (EDepthStencilState state) {
	switch (state) {
	case EDepthStencilState::Never:
		m_DepthTestEnable = false;
		m_DepthWriteEnable = false;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::Never;
		break;
	case EDepthStencilState::Equal:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = true;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::Equal;
		break;
	case EDepthStencilState::EqualNoWrite:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = false;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::Equal;
		break;
	case EDepthStencilState::GreaterEqual:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = true;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::GreaterEqual;
		break;
	case EDepthStencilState::GreaterEqualNoWrite:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = false;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::GreaterEqual;
		break;
	case EDepthStencilState::LessEqual:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = true;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::LessEqual;
		break;
	case EDepthStencilState::LessEqualNoWrite:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = false;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::LessEqual;
		break;
	case EDepthStencilState::Always:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = true;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::Always;
		break;
	case EDepthStencilState::AlwaysNoWrite:
		m_DepthTestEnable = true;
		m_DepthWriteEnable = false;
		m_StencilTestEnable = false;
		m_DepthCompareOp = rf::EDepthCompareOp::Always;
		break;
	default:
		break;
	}

	return *this;
}

bool rf::DepthState::operator==(const DepthState& other) const {
	return
		m_DepthTestEnable == other.m_DepthTestEnable &&
		m_DepthWriteEnable == other.m_DepthWriteEnable &&
		m_DepthCompareOp == other.m_DepthCompareOp;
}

bool rf::DepthState::operator!=(const DepthState& other) const {
	return
		m_DepthTestEnable != other.m_DepthTestEnable ||
		m_DepthWriteEnable != other.m_DepthWriteEnable ||
		m_DepthCompareOp != other.m_DepthCompareOp;
}