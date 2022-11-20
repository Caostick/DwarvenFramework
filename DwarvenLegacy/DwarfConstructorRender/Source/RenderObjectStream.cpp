#include "DwarfConstructorRender/RenderObjectStream.h"

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Log.h>

rf::RenderObjectStream::RenderObjectStream() {
	Reset();
}

void rf::RenderObjectStream::SetPipeline(rf::GraphicsPipelineId pipeline) {
	auto& state = GetCurrentState();
	const bool keepCurrent = (state.m_Count == 0);

	if (keepCurrent) {
		state.m_Pipeline = pipeline;
	} else {
		auto newState = state;
		newState.m_Pipeline = pipeline;
		m_States.emplace_back(newState);
	}
}

void rf::RenderObjectStream::SetMesh(rf::MeshId mesh) {
	auto& state = GetCurrentState();
	const bool keepCurrent = (state.m_Count == 0);

	if (keepCurrent) {
		state.m_Mesh = mesh;
	} else {
		auto newState = state;
		newState.m_Mesh = mesh;
		m_States.emplace_back(newState);
	}
}

void rf::RenderObjectStream::PushInstances(uint32 count /*= 1*/) {
	auto& state = GetCurrentState();

	DFAssert(state.m_Mesh != nullptr, "Can't add instances! Batch has no mesh set!");
	DFAssert(state.m_Pipeline != nullptr, "Can't add instances! Batch has no pipeline set!");

	GetCurrentState().m_Count += count;
}

auto rf::RenderObjectStream::GetStates() const -> const df::Vector<State>& {
	return m_States;
}

void rf::RenderObjectStream::Reset() {
	m_States.clear();

	State state = {};
	state.m_Count = 0;
	state.m_Pipeline = 0;
	state.m_Mesh = 0;

	m_States.emplace_back(state);
}

auto rf::RenderObjectStream::GetCurrentState()->State& {
	DFAssert(!m_States.empty(), "Can't get active state!");
	return m_States.back();
}