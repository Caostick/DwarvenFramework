#include <DwarfProfiling/Profiling.h>

#include <memory>

#if DF_PROFILER_OPTICK

void df::Optick::Create() {
	OPTICK_SET_MEMORY_ALLOCATOR(&df::Optick::Alloc, &df::Optick::Free,
		[]() { /* Do some TLS initialization here if needed */ });
}

void df::Optick::Destroy() {
	OPTICK_SHUTDOWN();
}

void* df::Optick::Alloc(size_t size) {
	return malloc(size);
}

void df::Optick::Free(void* ptr) {
	free(ptr);
}

#endif