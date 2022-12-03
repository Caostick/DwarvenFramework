#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#include <tuple>

namespace df {
	template<typename T>
	class Tokenizer;

	template<typename T>
	auto CountOf(const TStringView<T>& string, T symbol) -> size_t;

	template<typename T>
	auto CountOf(const TStringView<T>& string, const T* symbols) -> size_t;

	template<typename T>
	bool CompareNoCase(const TStringView<T>& a, const TStringView<T>& b);

	template<typename T>
	auto ToLower(const TStringView<T>& string) -> TString<T>;

	template<typename T>
	auto ToUpper(const TStringView<T>& string) -> TString<T>;

	template<typename T>
	auto Find(const TStringView<T>& string, T symbol) -> int;

	template<typename T>
	void Replace(TString<T>& string, T from, T to);

	template<typename T>
	auto Replaced(const TString<T>& string, T from, T to)->TString<T>;

	template<typename T>
	auto Replaced(const TString<T>& string, T from, const T* to)->TString<T>;

	template<typename T>
	auto Replaced(const TString<T>& string, const T* symbols, const T* to)->TString<T>;

	template<typename T>
	auto TakeLeft(const TStringView<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	bool EqualPart(const TString<T>& string, const TStringView<T>& substring);

	template<typename T>
	bool EqualPart(const TStringView<T>& string, const TStringView<T>& substring);

	template<typename T>
	auto TakeLeft(const TString<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto TakeRight(const TStringView<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto TakeRight(const TString<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto DropLeft(const TStringView<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto DropLeft(const TString<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto DropRight(const TStringView<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	auto DropRight(const TString<T>& string, size_t count)->TStringView<T>;

	template<typename T>
	void Split(const TStringView<T>& string, int index, TStringView<T>& outLeft, TStringView<T>& outRight);

	template<typename T>
	void Split(const TString<T>& string, int index, TStringView<T>& outLeft, TStringView<T>& outRight);

	template<typename T>
	auto Split(const TStringView<T>& string, int index) -> std::tuple<TStringView<T>, TStringView<T>>;

	template<typename T>
	auto Split(const TString<T>& string, int index)->std::tuple<TStringView<T>, TStringView<T>>;

	template<typename T>
	auto Trim(const TStringView<T>& string, const T* symb)->TStringView<T>;

	template<typename T>
	auto Trim(const TString<T>& string, const T* symb)->TStringView<T>;

	template<typename T>
	auto Tokenize(const TStringView<T>& string, const T* symbols)->Tokenizer<T>;

	template<typename T>
	auto Tokenize(const TString<T>& string, const T* symbols)->Tokenizer<T>;
}

#include <DwarvenCore/StringOperations.inl>
