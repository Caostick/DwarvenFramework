#include "VkShaderFileIncluder.h"
#include "VkParameterSetDefinition.h"
#include "VkShaderCompileInfo.h"
#include "VkRenderCore.h"

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/New.h>

vk::ShaderFileIncluder::ShaderFileIncluder(const vk::RenderCore& renderCore)
	: m_RenderCore(renderCore) {}

glslang::TShader::Includer::IncludeResult* vk::ShaderFileIncluder::includeLocal(
	const char* headerName,
	const char* /*includerName*/,
	size_t /*inclusionDepth*/) {

	const df::StringView includeName = df::StringView(headerName);

	const auto& code = m_RenderCore.GetShaderInclude(headerName);
	return DFNew IncludeResult(headerName, code.c_str(), code.size(), nullptr);
}

void vk::ShaderFileIncluder::releaseInclude(IncludeResult* result) {
	DFDelete result;
}