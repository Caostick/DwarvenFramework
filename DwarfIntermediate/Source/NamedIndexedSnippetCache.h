#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Lock.h>

namespace df {
    class NamedIndexedSnippetCache {
    public:
        NamedIndexedSnippetCache(uint32 maxNumIndices);

        void AddPrototype(const StringView& name, const StringView& snippet);
        auto Get(const StringView& name, uint32 idx) const -> const String&;

    private:
        struct InstanceStorage {
            Vector<String> m_Instances;
        };

        HashMap<String, InstanceStorage> m_Prototypes;
		String m_Empty;

        uint32 m_MaxNumIndices;
    };
}