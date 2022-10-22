#pragma once

#include <DwarfConstructorRender/ShaderGraph/NodeFactory.h>

template<typename T>
void rf::sg::ShaderGraph::RegisterNodeFactory(const df::StringView& name) {
	RegisterNodeFactory(name, DFNew TShaderGraphNodeFactory<T>);
}