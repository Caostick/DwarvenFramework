#pragma once

#include <stdint.h>

namespace df {
	void MemCpy(void* dst, const void* src, size_t bytes);

	template<typename T>
	auto MemAlign(T offset, T alignment)->T;

	template<typename T>
	void TMemCpy(T* dst, const T* src, size_t amount);

	template<typename T>
	int TMemCmp(const T* buf1, const T* buf2, size_t amount);

	template<typename T>
	void ZeroMemory(T* memory);
}

#include <DwarvenCore/Memory.inl>