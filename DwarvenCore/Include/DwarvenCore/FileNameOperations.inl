#pragma once

#include <DwarvenCore/StringOperations.h>

template<typename T>
auto df::GetFilename(const TStringView<T>& filePath) -> TStringView<T> {
	TStringView<T> filename;
	for (auto str : Tokenize(filePath, "/")) {
		filename = str;
	}

	return filename;
}

template<typename T>
auto df::GetExtension(const TStringView<T>& filePath) -> TStringView<T> {
	TStringView<T> filename;
	for (auto str : Tokenize(filePath, "/")) {
		filename = str;
	}

	TStringView<T> extension;
	for (auto str : Tokenize(filename, ".")) {
		if (str != filename) {
			extension = str;
		}
	}

	return extension;
}

template<typename T>
auto df::FixSlash(const TStringView<T>& filePath) -> TString<T> {
	TString<T> result = TString<T>(filePath);

	for (size_t i = 0; i < result.length(); ++i) {
		if (result[i] == '\\') {
			result[i] = '/';
		}
	}

	return result;
}

template<typename T>
auto df::OptimizePath(const TStringView<T>& filePath) -> TString<T> {
	const TString<T> fixedFilePath = FixSlash(filePath);

	TString<T> result;
	Vector<TStringView<T>> tokens;

	for (const auto token : Tokenize(TStringView<T>(fixedFilePath), "/")) {
		if (token == "..") {
			if (!tokens.empty()) {
				tokens.pop_back();
			} else {
				tokens.push_back(token);
			}
		} else if (token == ".") {
			if (tokens.empty()) {
				tokens.push_back(token);
			}
		} else {
			tokens.push_back(token);
		}
	}

	bool first = true;
	for (const auto& token : tokens) {
		if (!first) {
			result += "/";
		}
		result += TString<T>(token);

		first = false;
	}

	return result;
}