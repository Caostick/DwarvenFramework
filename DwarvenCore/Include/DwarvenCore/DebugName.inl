#pragma once

#if DF_USE_DEBUG_NAME

template <typename ...TArgs>
auto df::DebugName<TArgs...>(const df::StringView& tmp, TArgs... arg) -> df::String {
	const df::String tmpStr = df::String(tmp.data(), tmp.size());
	char nameBuffer[256];
	sprintf_s(nameBuffer, tmpStr.c_str(), arg...);
	return df::String(nameBuffer);
}

#else

template <typename ...TArgs>
auto df::DebugName<TArgs...>(const df::StringView&, TArgs...) -> const char* {
	return "";
}

#endif