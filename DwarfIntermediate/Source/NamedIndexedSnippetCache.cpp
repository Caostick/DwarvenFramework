#include "NamedIndexedSnippetCache.h"

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>

df::NamedIndexedSnippetCache::NamedIndexedSnippetCache(uint32 maxNumIndices) 
    : m_MaxNumIndices(maxNumIndices) {

}

void df::NamedIndexedSnippetCache::AddPrototype(const StringView& name, const StringView& snippet) {
    auto& storage = m_Prototypes[String(name)];
    const auto prototype = String(snippet);
    storage.m_Instances.resize(m_MaxNumIndices);

    for(size_t i=0; i< storage.m_Instances.size(); ++i) {
        storage.m_Instances[i] = Replaced<char>(prototype, '#', std::to_string(i).c_str());
    }
}

auto df::NamedIndexedSnippetCache::Get(const StringView& name, uint32 idx) const -> const String& {
    DFAssert(idx < m_MaxNumIndices, "Index is out of range!");

    auto it = m_Prototypes.find(String(name));
    if(it != m_Prototypes.end()) {
        return it->second.m_Instances[idx];
    }

    return m_Empty;
}