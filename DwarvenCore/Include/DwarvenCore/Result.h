#pragma once

namespace df {
	template<typename T, typename E>
	class Result {
	public:
		Result(const T& result);
		Result(const E& result);
		Result(const Result&) = default;
		Result(Result&&) = default;
		Result& operator = (const Result&) = default;
		Result& operator = (Result&&) = default;

		operator bool() const;

		auto GetResult() const -> const T&;
		auto GetError() const -> const E&;

	private:
		bool m_Succeded;

		T m_Result;
		E m_Error;
	};

	template<typename T>
	class ResultNoError {
	public:
		ResultNoError();
		ResultNoError(const T& result);
		ResultNoError(const ResultNoError&) = default;
		ResultNoError(ResultNoError&&) = default;
		ResultNoError& operator = (const ResultNoError&) = default;
		ResultNoError& operator = (ResultNoError&&) = default;

		operator bool() const;

		auto Get() const -> const T&;

	private:
		bool m_Succeded;

		T m_Result;
	};
}

#include <DwarvenCore/Result.inl>