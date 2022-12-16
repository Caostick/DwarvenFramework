#include "ShaderFileIncluder.h"
#include "ShaderIncludeCache.h"

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/New.h>

df::ShaderFileIncluder::ShaderFileIncluder(ShaderIncludeCache& includeCache)
	: m_IncludeCache(includeCache) {}

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