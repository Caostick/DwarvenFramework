#include <coroutine>
#include <string>

namespace df {
    class ResourceLoadingTask {
    public:
        struct promise_type;
        using Handle = std::coroutine_handle<promise_type>;

        ResourceLoadingTask(Handle handle);
        ResourceLoadingTask(ResourceLoadingTask&) = delete;
        ResourceLoadingTask(ResourceLoadingTask&&) = delete;
        ~ResourceLoadingTask();

        bool Process();
        auto GetRequestedResource() -> const std::string&;

    private:
        Handle m_Handle;

    public:
        struct promise_type {
            auto get_return_object() -> Handle;
            auto initial_suspend() noexcept -> std::suspend_always;
            auto final_suspend() noexcept -> std::suspend_always;
            void return_void();
            void unhandled_exception();

            auto yield_value(const std::string& value) -> std::suspend_always;

            std::string m_ResourceUUID;
        };
    };
}