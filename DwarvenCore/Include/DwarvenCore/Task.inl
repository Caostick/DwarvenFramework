#pragma once

#include <assert.h>

template<typename T>
df::TTask<T>::TTask(Handle handle)
    : m_Handle(handle) {

    assert(m_Handle);
}

template<typename T>
df::TTask<T>::~TTask() {
    m_Handle.destroy();
}

template<typename T>
df::TTask<T>::operator bool() const {
    return !m_Handle.done();
}

template<typename T>
auto df::TTask<T>::operator()() -> const T& {
    m_Handle.resume();
    return m_Handle.promise().m_Value;
}

template<typename T>
auto df::TTask<T>::Get() const -> const T& {
    return m_Handle.promise().m_Value;
}


template<typename T>
auto df::TTask<T>::promise_type::get_return_object() -> Handle {
    return Handle::from_promise(*this);
}

template<typename T>
auto df::TTask<T>::promise_type::initial_suspend() noexcept -> std::suspend_always {
    return std::suspend_always();
}

template<typename T>
auto df::TTask<T>::promise_type::final_suspend() noexcept -> std::suspend_always {
    return std::suspend_always();
}

template<typename T>
void df::TTask<T>::promise_type::unhandled_exception() {
    std::terminate();
}

template<typename T>
auto df::TTask<T>::promise_type::yield_value(T&& value) -> std::suspend_always {
    m_Value = std::move(value);
    return std::suspend_always();
}

template<typename T>
void df::TTask<T>::promise_type::return_value(T&& value) {
    m_Value = std::move(value);
}