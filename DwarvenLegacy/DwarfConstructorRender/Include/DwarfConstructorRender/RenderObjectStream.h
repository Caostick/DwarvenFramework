#pragma once

#include "DwarfConstructorRender/ObjectId.h"

#include <DwarvenCore/Vector.h>

namespace rf {
	class RenderObjectStream {
	public:
		struct State {
			rf::GraphicsPipelineId m_Pipeline;
			rf::MeshId m_Mesh;
			uint32 m_Count;
		};

		RenderObjectStream();

		void SetPipeline(rf::GraphicsPipelineId pipeline);
		void SetMesh(rf::MeshId mesh);

		void PushInstances(uint32 count = 1);

		auto GetStates() const -> const df::Vector<State>&;
		void Reset();

	private:
		auto GetCurrentState()->State&;

		df::Vector<State> m_States;
	};
}