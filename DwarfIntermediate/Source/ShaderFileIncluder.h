#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

#include <glslang/Public/ShaderLang.h>

namespace df {
    class ShaderIncludeCache;
    class NamedIndexedSnippetCache;

    struct PipelineCompileInfo;
}

namespace df {
	class ShaderFileIncluder : public glslang::TShader::Includer {
	public:
		ShaderFileIncluder(
            PipelineCompileInfo& compileInfo,
            ShaderIncludeCache& includeCache,
            const NamedIndexedSnippetCache& vertexAttributeSnippetCache,
            const NamedIndexedSnippetCache& parameterSetSnippetCache);

		auto includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) -> IncludeResult* override;
		void releaseInclude(IncludeResult* result) override;

	private:
        PipelineCompileInfo& m_CompileInfo;
		ShaderIncludeCache& m_IncludeCache;
        const NamedIndexedSnippetCache& m_VertexAttributeSnippetCache;
		const NamedIndexedSnippetCache& m_ParameterSetSnippetCache;
	};
}