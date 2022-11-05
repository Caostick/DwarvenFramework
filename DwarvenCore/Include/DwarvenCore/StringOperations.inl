#include <algorithm>

#include <DwarvenCore/Vector.h>

template<typename T>
auto df::CountOf(const TStringView<T>& string, T symbol) -> size_t {
	size_t count = 0;
	for (auto c : string) {
		count += (c == symbol) ? 1 : 0;
	}
	return count;
}

template<typename T>
auto df::CountOf(const TStringView<T>& string, const T* symbols) -> size_t {
	size_t count = 0;

	const df::TStringView<T> symbView(symbols);
	for (auto c : symbView) {
		count += CountOf(string, c);
	}
	return count;
}

template<typename T>
bool df::CompareNoCase(const TStringView<T>& a, const TStringView<T>& b) {
	if (a.length() != b.length()) {
		return false;
	}

	for (size_t i = 0; i < a.length(); ++i) {
		if (std::tolower(static_cast<int>(a[i])) != std::tolower(static_cast<int>(b[i]))) {
			return false;
		}
	}

	return true;
}

template<typename T>
auto df::Find(const TStringView<T>& string, T symbol) -> int {
	int idx = 0;
	for (auto c : string) {
		if (c == symbol) {
			return idx;
		}
		idx++;
	}
	return -1;
}

template<typename T>
void df::Replace(TString<T>& string, T from, T to) {
	for (auto& c : string) {
		if (c == from) {
			c = to;
		}
	}
}

template<typename T>
auto df::Replaced(const TString<T>& string, T from, T to)->TString<T> {
	TString<T> result = string;
	Replace(result, from, to);
	return result;
}

template<typename T>
auto df::Replaced(const TString<T>& string, T from, const T* to)->TString<T> {
	if (to[0] != 0 && to[1] == 0) {
		return Replaced(string, from, to[0]);
	}

	const char separator[] = { from, 0 };

	df::Vector<TStringView<T>> tokens;
	for (const auto token : df::Tokenize<T>(string, separator)) {
		tokens.push_back(token);
	}

	const size_t numTokens = tokens.size();
	if (numTokens < 2) {
		return string;
	}

	const TString<T> strTo = to;

	TString<T> result;
	result.reserve(string.size() + (strTo.size() - 1) * (numTokens - 1));
	for (size_t i = 0; i < numTokens - 1; ++i) {
		result += tokens[i];
		result += strTo;
	}
	result += tokens[numTokens - 1];

	return result;
}

template<typename T>
auto df::Replaced(const TString<T>& string, const T* symbols, const T* to)->TString<T> {
	df::Vector<TStringView<T>> tokens;
	for (const auto token : df::Tokenize<T>(string, symbols)) {
		tokens.push_back(token);
	}

	const size_t numTokens = tokens.size();
	if (numTokens < 2) {
		return string;
	}

	const TString<T> strTo = to;

	TString<T> result;
	result.reserve(string.size() + (strTo.size() - 1) * (numTokens - 1));
	for (size_t i = 0; i < numTokens - 1; ++i) {
		result += tokens[i];
		result += strTo;
	}
	result += tokens[numTokens - 1];

	return result;
}

template<typename T>
df::TStringView<T> df::TakeLeft(const df::TStringView<T>& string, size_t count) {
	return string.substr(0, std::min(string.length(), count));
}

template<typename T>
bool df::EqualPart(const TString<T>& string, const TStringView<T>& substring) {
	if (string.length() < substring.length()) {
		return false;
	}

	return TakeLeft(string, substring.length()) == substring;
}

template<typename T>
bool df::EqualPart(const TStringView<T>& string, const TStringView<T>& substring) {
	if (string.length() < substring.length()) {
		return false;
	}

	return TakeLeft(string, substring.length()) == substring;
}

template<typename T>
df::TStringView<T> df::TakeLeft(const TString<T>& string, size_t count) {
	return TakeLeft(TStringView<T>(string), count);
}

template<typename T>
df::TStringView<T> df::TakeRight(const df::TStringView<T>& string, size_t count) {
	const size_t length = string.length();
	const size_t minLen = std::min(length, count);
	const size_t bias = length - minLen;

	return string.substr(bias, minLen);
}

template<typename T>
df::TStringView<T> df::TakeRight(const df::TString<T>& string, size_t count) {
	return TakeRight(TStringView<T>(string), count);
}

template<typename T>
df::TStringView<T> df::DropLeft(const df::TStringView<T>& string, size_t count) {
	const size_t length = string.length();
	return df::TakeRight(string, length - std::min(length, count));
}

template<typename T>
df::TStringView<T> df::DropLeft(const df::TString<T>& string, size_t count) {
	return DropLeft(TStringView<T>(string), count);
}

template<typename T>
df::TStringView<T> df::DropRight(const df::TStringView<T>& string, size_t count) {
	const size_t length = string.length();
	return df::TakeLeft(string, length - std::min(length, count));
}

template<typename T>
df::TStringView<T> df::DropRight(const df::TString<T>& string, size_t count) {
	return DropRight(TStringView<T>(string), count);
}

template<typename T>
void df::Split(const df::TStringView<T>& string, int index, df::TStringView<T>& outLeft, df::TStringView<T>& outRight) {
	outLeft = df::TakeLeft(string, index);
	outRight = df::DropLeft(string, index);
}

template<typename T>
void df::Split(const df::TString<T>& string, int index, df::TStringView<T>& outLeft, df::TStringView<T>& outRight) {
	Split(TStringView<T>(string), index, outLeft, outRight);
}

template<typename T>
auto df::Split(const TStringView<T>& string, int index)->std::tuple<TStringView<T>, TStringView<T>> {
	return std::make_tuple(
		df::TakeLeft(string, index),
		df::DropLeft(string, index)
	);
}

template<typename T>
auto df::Split(const TString<T>& string, int index)->std::tuple<TStringView<T>, TStringView<T>> {
	return std::make_tuple(
		df::TakeLeft(string, index),
		df::DropLeft(string, index)
	);
}

template<typename T>
df::TStringView<T> df::Trim(const df::TStringView<T>& string, const T* symb) {
	const size_t length = string.length();
	const size_t symbCount = df::TStringView<T>(symb).length();

	if (length == 0) {
		return df::TStringView<T>();
	}

	for (size_t i = 0; i < symbCount; ++i) {
		if (string[0] == symb[i]) {
			return df::Trim(df::DropLeft(string, 1), symb);
		} else if (string[length - 1] == symb[i]) {
			return df::Trim(df::DropRight(string, 1), symb);
		}
	}

	return string;
}

template<typename T>
df::TStringView<T> df::Trim(const df::TString<T>& string, const T* symb) {
	return Trim(TStringView<T>(string), symb);
}

template<typename T>
df::Tokenizer<T> df::Tokenize(const df::TStringView<T>& string, const T* symbols) {
	return df::Tokenizer<T>(string, symbols);
}

template<typename T>
df::Tokenizer<T> df::Tokenize(const TString<T>& string, const T* symbols) {
	return df::Tokenizer<T>(string, symbols);
}


namespace df {
	template<typename T>
	class Tokenizer {
	public:
		template<typename T>
		class Iterator {
		public:
			Iterator() = delete;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;

			Iterator(const df::TStringView<T>& string, const df::TStringView<T>& separators)
				: m_String(string.data())
				, m_Count(string.length())
				, m_Separators(separators) {}

			Iterator& operator++() {
				for (int i = 0; i < m_Count; ++i) {
					for (const T sep : m_Separators) {
						if (m_String[i] == sep) {
							m_String += (i + 1);
							m_Count -= (i + 1);
							return *this;
						}
					}
				}

				m_Count = 0;
				m_String = nullptr;
				return *this;
			}

			bool operator != (const Iterator&) const {
				return m_String != nullptr;
			}

			df::TStringView<T> operator* () const {
				for (int i = 0; i < m_Count; ++i) {
					for (const T sep : m_Separators) {
						if (m_String[i] == sep) {
							return df::StringView(m_String, i);
						}
					}
				}

				return df::StringView(m_String, m_Count);
			}

		private:
			size_t m_Count = 0;
			const T* m_String = nullptr;
			const df::TStringView<T>& m_Separators;
		};

	public:
		Tokenizer() = delete;
		Tokenizer(const Tokenizer&) = default;
		Tokenizer(Tokenizer&&) = default;

		Tokenizer(const df::TStringView<T>& string, const df::TStringView<T>& separators)
			: m_String(string)
			, m_Separators(separators) {}

		Iterator<T> begin() {
			return Iterator<T>(m_String, m_Separators);
		}

		Iterator<T> end() {
			return Iterator<T>(m_String, m_Separators);
		}
	private:
		df::TStringView<T> m_String;
		df::TStringView<T> m_Separators;
	};
}