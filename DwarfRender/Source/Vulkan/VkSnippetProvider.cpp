#include "VkSnippetProvider.h"

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>

namespace {
    constexpr size_t MaxNumVertexAttributes = 16;
    constexpr size_t MaxNumParameterSets = 16;
}

void vk::SnippetProvider::AddVertexAttributePrototype(const df::StringView& name, const df::StringView& snippet) {
    auto& storage = m_VertexAttributePrototypes[df::String(name)];
    const auto prototype = df::String(snippet);
    storage.m_Instances.resize(MaxNumVertexAttributes);

    for (size_t i = 0; i < storage.m_Instances.size(); ++i) {
        storage.m_Instances[i] = df::Replaced<char>(prototype, '#', std::to_string(i).c_str());
    }
}

void vk::SnippetProvider::AddParameterSetPrototype(const df::StringView& name, const df::StringView& snippet) {
    df::WriteLock rLock(m_LockStorage);

    auto& storage = m_ParameterSetPrototypes[df::String(name)];
    const auto prototype = df::String(snippet);
    storage.m_Instances.resize(MaxNumParameterSets);

    for (size_t i = 0; i < storage.m_Instances.size(); ++i) {
        storage.m_Instances[i] = df::Replaced<char>(prototype, '#', std::to_string(i).c_str());
    }
}

auto vk::SnippetProvider::GetVertexAttributeSnippet(const df::StringView& name, uint32 idx) const -> const df::String& {
    DFAssert(idx < MaxNumVertexAttributes, "Index is out of range!");

    auto it = m_VertexAttributePrototypes.find(df::String(name));
    if (it != m_VertexAttributePrototypes.end()) {
        return it->second.m_Instances[idx];
    }

    return m_Empty;
}

auto vk::SnippetProvider::GetParameterSetSnippet(const df::StringView& name, uint32 idx) const -> const df::String& {
    DFAssert(idx < MaxNumParameterSets, "Index is out of range!");

    df::ReadLock rLock(m_LockStorage);

    auto it = m_ParameterSetPrototypes.find(df::String(name));
    if (it != m_ParameterSetPrototypes.end()) {
        return it->second.m_Instances[idx];
    }

    return m_Empty;
}