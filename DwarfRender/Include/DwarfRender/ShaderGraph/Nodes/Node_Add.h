#pragma once

#include <DwarfRender/ShaderGraph/Node.h>

namespace rf {
	namespace sg {
		class Node_Add : public Node {
		public:
			void MakeBindings() override;
			void MakeVariable(PipelineGenerator& pipeline) override;

		private:
			Node* m_A = nullptr;
			Node* m_B = nullptr;
		};
	}
}