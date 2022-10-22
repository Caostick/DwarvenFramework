#include <DwarfRender/RasterizationState.h>

df::RasterizationState::RasterizationState(ERasterizationState state) {
	*this = state;
}

df::RasterizationState& df::RasterizationState::operator = (ERasterizationState state) {
	m_CullMode = ECullMode::Back;
	m_FrontFace = EFrontFace::CounterClockwise;
	m_RasterizerDiscardEnable = false;

	switch (state) {
	case ERasterizationState::Fill:
		m_PolygonMode = EPolygonMode::Fill;
		break;
	case ERasterizationState::Line:
		m_PolygonMode = EPolygonMode::Line;
		break;
	case ERasterizationState::Point:
		m_PolygonMode = EPolygonMode::Point;
		break;
	default:
		break;
	}

	return *this;
}

bool df::RasterizationState::operator==(const RasterizationState& other) const {
	return
		m_RasterizerDiscardEnable == other.m_RasterizerDiscardEnable &&
		m_PolygonMode == other.m_PolygonMode &&
		m_FrontFace == other.m_FrontFace &&
		m_CullMode == other.m_CullMode;
}

bool df::RasterizationState::operator!=(const RasterizationState& other) const {
	return
		m_RasterizerDiscardEnable != other.m_RasterizerDiscardEnable ||
		m_PolygonMode != other.m_PolygonMode ||
		m_FrontFace != other.m_FrontFace ||
		m_CullMode != other.m_CullMode;
}