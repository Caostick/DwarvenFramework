#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

#include <glslang/Public/ShaderLang.h>

namespace df {
	class ResourceManager;
}

namespace rf {
	struct ShaderCompileInfo;
	class ParamSetDefinitionManager;
}

namespace rf {
	class ShaderFileIncluder : public glslang::TShader::Includer {
	public:
		ShaderFileIncluder(
			const df::ResourceManager& resourceManager,
			const rf::ParamSetDefinitionManager& paramSetDefinitionManager,
			rf::ShaderCompileInfo& info);

		auto includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) -> IncludeResult* override;
		void releaseInclude(IncludeResult* result) override;

	private:
		const df::ResourceManager& m_ResourceManager;
		const rf::ParamSetDefinitionManager& m_ParamSetDefinitionManager;
		rf::ShaderCompileInfo& m_ShaderCompileInfo;
	};
}