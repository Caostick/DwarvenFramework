#pragma once 

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Types.h>

// @TODO(!!!): Make shader snippet provider!

namespace df {
    class VertexAttributeHandler {
    public:
        virtual void CreateVertexAttributePrototype(const StringView& name, const StringView& snippet) = 0;
    };

    class ParameterSetHandler {
    public:
        virtual void CreateParameterSetPrototype(const StringView& name, const StringView& snippet) = 0;
    };

    class SnippetProvider {
    public:
        virtual ~SnippetProvider() {}

        virtual auto GetVertexAttributeSnippet(const StringView& name, uint32 idx) const -> const String& = 0;
        virtual auto GetParameterSetSnippet(const StringView& name, uint32 idx) const -> const String& = 0;
    };
}