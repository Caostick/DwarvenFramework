#include <DwarvenCore/Memory.h>

#include <algorithm>
#include <memory>

void df::MemCpy(void* dst, const void* src, size_t bytes) {
	memcpy_s(dst, bytes, src, bytes);
}
