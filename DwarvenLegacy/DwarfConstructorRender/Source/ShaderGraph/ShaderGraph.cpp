#include <DwarfConstructorRender/ShaderGraph/ShaderGraph.h>
#include <DwarfConstructorRender/ShaderGraph/Node.h>

#include <DwarfConstructorRender/ShaderGraph/NodeFactory.h>

#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float.h>
#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float2.h>
#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float3.h>
#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Float4.h>
#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Add.h>

#include <DwarfConstructorRender/ParamSetDeclarator.h>
#include <DwarfConstructorRender/ParamSetDefinition.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

bool rf::sg::ShaderGraph::Init() {
	RegisterNodeFactory<Node_Float>("Float");
	RegisterNodeFactory<Node_Float2>("Float2");
	RegisterNodeFactory<Node_Float3>("Float3");
	RegisterNodeFactory<Node_Float4>("Float4");
	RegisterNodeFactory<Node_Add>("Add");

	return true;
}

void rf::sg::ShaderGraph::Release() {
	Reset();

	for (auto& it : m_Factories) {
		DFDelete it.second;
	}
	m_Factories.clear();
}

void rf::sg::ShaderGraph::MakeParamSetDefinition(rf::ParamSetDefinition& paramSetDefinition) {
	rf::ParamSetDeclarator declarator(paramSetDefinition);

	for (auto node : m_Nodes) {
		if (node->IsParameter()) {
			node->AddParameter(declarator);
		}
	}
}

void rf::sg::ShaderGraph::Reset() {
	for (auto node : m_Nodes) {
		DFDelete node;
	}
	m_Nodes.clear();
}

void rf::sg::ShaderGraph::ResetVariables() {
	for (auto node : m_Nodes) {
		node->ResetVariable();
	}
}

auto rf::sg::ShaderGraph::CreateNode(const df::StringView& name)->Node* {
	const auto it = m_Factories.find(df::String(name));
	if (it == m_Factories.end()) {
		return nullptr;
	}

	const NodeFactory* factory = it->second;
	if (!factory) {
		return nullptr;
	}

	Node* node = factory->Create();
	m_Nodes.push_back(node);

	return node;
}

void rf::sg::ShaderGraph::RegisterNodeFactory(const df::StringView& name, const NodeFactory* factory) {
	const df::String prototypeName = df::String(name);

	const auto it = m_Factories.find(prototypeName);
	if (it != m_Factories.end()) {
		DFAssert(false, "Material node prototype already registered!");
	}

	m_Factories[prototypeName] = factory;
}