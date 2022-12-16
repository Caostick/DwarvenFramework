#pragma once

#include <DwarvenCore/Types.h>

#include <vector>

namespace df {
	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	auto AddUnique(Vector<T>& vector, const T& element) -> uint32;

	template<typename T>
	auto FindElement(Vector<T>& vector, const T& element) -> int;

	template<typename T>
	auto RemoveOrdered(Vector<T>& vector, const T& element) -> int;

	template<typename T>
	auto RemoveUnordered(Vector<T>& vector, const T& element) -> int;

	#include <DwarvenCore/Vector.inl>
}