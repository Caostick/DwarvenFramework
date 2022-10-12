#include <DwarfRender/RasterizationState.h>

rf::RasterizationState::RasterizationState(ERasterizationState state) {
	*this = state;
}

rf::RasterizationState& rf::RasterizationState::operator = (ERasterizationState state) {
	m_CullMode = rf::ECullMode::Back;
	m_FrontFace = rf::EFrontFace::CounterClockwise;
	m_RasterizerDiscardEnable = false;

	switch (state) {
	case rf::ERasterizationState::Fill:
		m_PolygonMode = rf::EPolygonMode::Fill;
		break;
	case rf::ERasterizationState::Line:
		m_PolygonMode = rf::EPolygonMode::Line;
		break;
	case rf::ERasterizationState::Point:
		m_PolygonMode = rf::EPolygonMode::Point;
		break;
	default:
		break;
	}

	return *this;
}

bool rf::RasterizationState::operator==(const RasterizationState& other) const {
	return
		m_RasterizerDiscardEnable == other.m_RasterizerDiscardEnable &&
		m_PolygonMode == other.m_PolygonMode &&
		m_FrontFace == other.m_FrontFace &&
		m_CullMode == other.m_CullMode;
}

bool rf::RasterizationState::operator!=(const RasterizationState& other) const {
	return
		m_RasterizerDiscardEnable != other.m_RasterizerDiscardEnable ||
		m_PolygonMode != other.m_PolygonMode ||
		m_FrontFace != other.m_FrontFace ||
		m_CullMode != other.m_CullMode;
}