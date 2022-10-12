#pragma once

#include <DwarvenCore/New.h>

template<typename T>
auto rf::sg::TShaderGraphNodeFactory<T>::Create() const->Node* {
	T* node = DFNew T;
	node->MakeBindings();
	return node;
}