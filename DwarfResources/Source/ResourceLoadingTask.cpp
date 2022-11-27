#include <DwarfResources/ResourceLoadingTask.h>

#include <assert.h>

df::ResourceLoadingTask::ResourceLoadingTask(Handle handle)
    : m_Handle(handle) {

    assert(m_Handle);
}

df::ResourceLoadingTask::~ResourceLoadingTask() {
    m_Handle.destroy();
}

bool df::ResourceLoadingTask::Process() {
    if (!m_Handle.done())     {
        m_Handle.resume();
    }
    return !m_Handle.done();
}

auto df::ResourceLoadingTask::GetRequestedResource() -> const std::string& {
    return m_Handle.promise().m_ResourceUUID;
}



auto df::ResourceLoadingTask::promise_type::get_return_object() -> Handle {
    return Handle::from_promise(*this);
}

auto df::ResourceLoadingTask::promise_type::initial_suspend() noexcept -> std::suspend_always {
    return std::suspend_always(); 
}

auto df::ResourceLoadingTask::promise_type::final_suspend() noexcept -> std::suspend_always {
    return std::suspend_always(); 
}

void df::ResourceLoadingTask::promise_type::return_void() {

}

void df::ResourceLoadingTask::promise_type::unhandled_exception() {
    std::terminate();
}

auto df::ResourceLoadingTask::promise_type::yield_value(const std::string &value) -> std::suspend_always {
    m_ResourceUUID = value;
    return std::suspend_always();
}