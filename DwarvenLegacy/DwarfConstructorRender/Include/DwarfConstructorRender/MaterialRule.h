#pragma once

#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Vector.h>

namespace rf {
	class ParamSetDefinition;

	namespace sg {
		class ShaderGraph;
	}
}

namespace rf {
	class MaterialRule {
	public:
		ParamSetDefinition* m_ParametersDefinition = nullptr;
		rf::MaterialId m_DefaultMaterial = nullptr;
		sg::ShaderGraph* m_ShaderGraph = nullptr;
		df::Vector<rf::PipelineId> m_Pipelines;
	};
}