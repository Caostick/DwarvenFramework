#include "ShaderFileIncluder.h"
#include "ShaderIncludeCache.h"
#include "NamedIndexedSnippetCache.h"

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/New.h>

df::ShaderFileIncluder::ShaderFileIncluder(
    PipelineCompileInfo& compileInfo,
    ShaderIncludeCache& includeCache,
    const NamedIndexedSnippetCache& vertexAttributeSnippetCache,
    const NamedIndexedSnippetCache& parameterSetSnippetCache)
    : m_CompileInfo(compileInfo)
	, m_IncludeCache(includeCache) 
    , m_VertexAttributeSnippetCache(vertexAttributeSnippetCache)
    , m_ParameterSetSnippetCache(parameterSetSnippetCache) {}

glslang::TShader::Includer::IncludeResult* df::ShaderFileIncluder::includeLocal(
	const char* headerName,
	const char* /*includerName*/,
	size_t /*inclusionDepth*/) {

	const df::StringView includeName = df::StringView(headerName);

	const auto& code = m_IncludeCache.Get(headerName);
	return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);
}

void df::ShaderFileIncluder::releaseInclude(IncludeResult* result) {
	DFDelete result;
}