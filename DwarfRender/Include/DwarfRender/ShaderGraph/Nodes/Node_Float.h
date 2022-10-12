#pragma once

#include <DwarfRender/ShaderGraph/Node.h>

namespace rf {
	namespace sg {
		class Node_Float : public Node {
		public:
			void MakeBindings() override;
			void MakeVariable(PipelineGenerator& pipeline) override;
			void AddParameter(ParamSetDeclarator& paramDeclarator) override;
			bool IsParameter() const override;

		private:
			float m_Value = 0.0f;
			df::String m_ParameterName;
		};
	}
}