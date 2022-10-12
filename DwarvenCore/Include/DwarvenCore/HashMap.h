#pragma once

#include <unordered_map>

namespace df {
	template<typename K, typename T>
	using HashMap = std::unordered_map<K, T>;
}