#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>

#include <glslang/Public/ShaderLang.h>

namespace vk {
	class RenderCore;

	struct ShaderCompileInfo;
}

namespace vk {
	class ShaderFileIncluder : public glslang::TShader::Includer {
	public:
		ShaderFileIncluder(
			const vk::RenderCore& renderCore,
			vk::ShaderCompileInfo& info);

		auto includeLocal(const char* headerName, const char* includerName, size_t inclusionDepth) -> IncludeResult* override;
		void releaseInclude(IncludeResult* result) override;

	private:
		const vk::RenderCore& m_RenderCore;
		vk::ShaderCompileInfo& m_CompileInfo;
	};
}