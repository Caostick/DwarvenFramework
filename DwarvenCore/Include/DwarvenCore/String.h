#pragma once

#include <string>

namespace df {
	template<typename T>
	using TString = std::basic_string<T, std::char_traits<T>>;

	using String = TString<char>;
	using WString = TString<wchar_t>;
	using U32String = TString<char32_t>;
}