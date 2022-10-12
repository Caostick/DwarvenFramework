#include <DwarvenCore/DebugName.h>

#if DF_USE_DEBUG_NAME

auto df::DebugName(const df::StringView& name) -> const df::StringView& {
	return name;
}

#endif