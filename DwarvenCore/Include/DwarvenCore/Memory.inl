#pragma once

#include <memory.h>

template<typename T>
void df::TMemCpy(T* dst, const T* src, size_t amount) {
	df::MemCpy(dst, src, sizeof(T) * amount);
}

template<typename T>
int df::TMemCmp(const T* buf1, const T* buf2, size_t amount) {
	return memcmp(buf1, buf2, amount * sizeof(T));
}

template<typename T>
void df::ZeroMemory(T* memory) {
	memset(memory, 0, sizeof(T));
}