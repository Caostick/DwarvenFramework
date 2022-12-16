#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

#include <glslang/Public/ShaderLang.h>

namespace df {
    class ShaderIncludeCache;
}

namespace df {
	class ShaderFileIncluder : public glslang::TShader::Includer {
	public:
		ShaderFileIncluder(ShaderIncludeCache& includeCache);

		auto includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) -> IncludeResult* override;
		void releaseInclude(IncludeResult* result) override;

	private:
		ShaderIncludeCache& m_IncludeCache;
	};
}