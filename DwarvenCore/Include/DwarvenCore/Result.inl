#pragma once

template<typename T, typename E>
df::Result<T, E>::Result(const T& result)
	: m_Succeded(true)
	, m_Result(result) {}

template<typename T, typename E>
df::Result<T, E>::Result(const E& result)
	: m_Succeded(false)
	, m_Error(result) {}

template<typename T, typename E>
df::Result<T, E>::operator bool() const {
	return m_Succeded;
}

template<typename T, typename E>
auto df::Result<T, E>::GetResult() const -> const T& {
	return m_Result;
}

template<typename T, typename E>
auto df::Result<T, E>::GetError() const -> const E& {
	return m_Error;
}


template<typename T>
df::ResultNoError<T>::ResultNoError() 
	: m_Succeded(false) {
}

template<typename T>
df::ResultNoError<T>::ResultNoError(const T& result) 
	: m_Succeded(true)
	, m_Result(result) {
}

template<typename T>
df::ResultNoError<T>::operator bool() const {
	return m_Succeded;
}

template<typename T>
auto df::ResultNoError<T>::Get() const -> const T& {
	return m_Result;
}