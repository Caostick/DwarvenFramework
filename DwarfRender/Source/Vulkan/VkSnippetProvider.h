#pragma once

#include <DwarfRender/SnippetProvider.h>

#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Lock.h>

namespace vk {
    class SnippetProvider : public df::SnippetProvider {
    public:

        void AddVertexAttributePrototype(const df::StringView& name, const df::StringView& snippet);
        void AddParameterSetPrototype(const df::StringView& name, const df::StringView& snippet);

        virtual auto GetVertexAttributeSnippet(const df::StringView& name, uint32 idx) const -> const df::String & override;
        virtual auto GetParameterSetSnippet(const df::StringView& name, uint32 idx) const -> const df::String & override;

    private:
        struct InstanceStorage {
            df::Vector<df::String> m_Instances;
        };

        df::HashMap<df::String, InstanceStorage> m_VertexAttributePrototypes;
        df::HashMap<df::String, InstanceStorage> m_ParameterSetPrototypes;
        df::String m_Empty;

        mutable df::Lock m_LockStorage;
    };
}