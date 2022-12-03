#include <coroutine>
#include <string>

namespace df {
    // @TODO: Make for void and primitive types
    template<typename T>
    class TTask {
    public:
        struct promise_type;
        using Handle = std::coroutine_handle<promise_type>;

        TTask(Handle handle);
        TTask(TTask&) = delete;
        TTask(TTask&&) = delete;
        ~TTask();

        operator bool() const;
        auto operator()() -> const T&;
        auto Get() const -> const T&;

    private:
        Handle m_Handle;

    public:
        struct promise_type {
            auto get_return_object()->Handle;
            auto initial_suspend() noexcept -> std::suspend_always;
            auto final_suspend() noexcept -> std::suspend_always;
            void unhandled_exception();

            auto yield_value(T&& value)->std::suspend_always;
            void return_value(T&& value);

            T m_Value;
        };
    };
}

#include <DwarvenCore/Task.inl>