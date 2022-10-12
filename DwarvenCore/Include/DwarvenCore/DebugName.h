#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#define DF_USE_DEBUG_NAME 1

namespace df {
#if DF_USE_DEBUG_NAME

	template <typename ...TArgs>
	auto DebugName(const df::StringView& tmp, TArgs... arg)->df::String;

	auto DebugName(const df::StringView& name) -> const df::StringView&;

#else

	template <typename ...TArgs>
	auto DebugName(const df::StringView&, TArgs...) -> const char*;

#endif
}

#include <DwarvenCore/DebugName.inl>