#pragma once

#include <DwarfRender/ShaderGraph/Node.h>

namespace rf {
	namespace sg {
		class Node_Float3 : public Node {
		public:
			void MakeBindings() override;
			void MakeVariable(PipelineGenerator& pipeline) override;
			void AddParameter(ParamSetDeclarator& paramDeclarator) override;
			bool IsParameter() const override;

		private:
			Vec3 m_Value;
			df::String m_ParameterName;
		};
	}
}