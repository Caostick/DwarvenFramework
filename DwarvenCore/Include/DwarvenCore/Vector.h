#pragma once

#include <vector>

namespace df {
	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	void AddUnique(Vector<T>& vector, const T& element);

	template<typename T>
	auto FindElement(Vector<T>& vector, const T& element) -> int;

	template<typename T>
	auto RemoveOrdered(Vector<T>& vector, const T& element) -> int;

	template<typename T>
	auto RemoveUnordered(Vector<T>& vector, const T& element) -> int;

	#include <DwarvenCore/Vector.inl>
}