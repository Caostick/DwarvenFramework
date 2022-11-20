#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/New.h>

namespace rf {
	class ParamSetDefinition;

	namespace sg {
		class Node;
		class NodeFactory;
	}
}

namespace rf {
	namespace sg {
		class ShaderGraph {
		public:
			ShaderGraph() = default;
			virtual ~ShaderGraph() {}

			bool Init();
			void Release();

			void MakeParamSetDefinition(rf::ParamSetDefinition& paramSetDefinition);

			void Reset();
			void ResetVariables();

			template<typename T>
			void RegisterNodeFactory(const df::StringView& name);

			auto CreateNode(const df::StringView& name)->Node*;

			virtual auto GetOutputNode()->Node* = 0;
			virtual void CreateDefault() = 0;

		protected:
			void RegisterNodeFactory(const df::StringView& name, const NodeFactory* factory);

			df::HashMap<df::String, const NodeFactory*> m_Factories;
			df::Vector<Node*> m_Nodes;
		};
	}
}

#include <DwarfConstructorRender/ShaderGraph/ShaderGraph.inl>