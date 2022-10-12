#pragma once

#include <string_view>

namespace df {
	template<typename T>
	using TStringView = std::basic_string_view<T>;

	using StringView = TStringView<char>;
	using WStringView = TStringView<wchar_t>;
}