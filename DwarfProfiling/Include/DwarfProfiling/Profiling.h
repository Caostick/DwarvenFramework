#pragma once

#if DF_PROFILER_OPTICK

#include <optick.h>

namespace df {
	class Optick {
	public:
		static void Create();
		static void Destroy();
		static auto Alloc(size_t size) -> void*;
		static void Free(void* mem);
	};
}

#define DF_PROFILE_CREATE() df::Optick::Create()
#define DF_PROFILE_DESTROY() df::Optick::Destroy()
#define DF_PROFILE_EVENT(name) OPTICK_EVENT(name)
#define DF_PROFILE_THREAD(name) OPTICK_THREAD(name)
#define DF_PROFILE_FRAME(name) OPTICK_FRAME(name)
#define DF_PROFILE_FUNCTION() DF_PROFILE_EVENT(__FUNCTION__)

#else

#define DF_PROFILE_CREATE()
#define DF_PROFILE_DESTROY()
#define DF_PROFILE_EVENT(name)
#define DF_PROFILE_THREAD(name)
#define DF_PROFILE_FRAME(name)
#define DF_PROFILE_FUNCTION()

#endif