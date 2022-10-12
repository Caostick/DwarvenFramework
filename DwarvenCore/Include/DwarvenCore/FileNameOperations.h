#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

namespace df {
	template<typename T>
	auto GetFilename(const TStringView<T>& filePath) -> TStringView<T>;

	template<typename T>
	auto GetExtension(const TStringView<T>& filePath) -> TStringView<T>;

	template<typename T>
	auto FixSlash(const TStringView<T>& filePath) -> TString<T>;

	template<typename T>
	auto OptimizePath(const TStringView<T>& filePath) -> TString<T>;
}

#include <DwarvenCore/FileNameOperations.inl>