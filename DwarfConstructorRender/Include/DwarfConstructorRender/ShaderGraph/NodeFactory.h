#pragma once

namespace rf {
	namespace sg {
		class Node;
	}
}

namespace rf {
	namespace sg {
		class NodeFactory {
		public:
			virtual ~NodeFactory() {}
			virtual auto Create() const->Node* = 0;
		};

		template<typename T>
		class TShaderGraphNodeFactory : public NodeFactory {
		public:
			auto Create() const->Node* override;
		};
	}
}

#include <DwarfConstructorRender/ShaderGraph/NodeFactory.inl>