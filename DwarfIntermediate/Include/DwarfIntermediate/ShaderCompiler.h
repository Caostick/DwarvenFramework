#pragma once

#include <DwarfRender/SnippetProvider.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Result.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Renderer;
	class FileSystem;
	class ShaderIncludeCache;
	class SnippetProvider;

	//struct PipelineCompileInfo;
}

namespace df {
	enum class EShaderType {
		Vertex,
		Geometry,
		Fragment,
		Compute,
		Mesh
	};
}

namespace df {
	using ShaderCompileResult = Result<Vector<uint32>, String>;

	class ShaderCompiler {
	public:
		ShaderCompiler(const Renderer& renderer, FileSystem& fileSystem, const StringView& dataPath);
		~ShaderCompiler();

		auto GetVertexAttribute(const StringView& name, uint32 idx) const -> const String&;
		auto GetParameterSet(const StringView& name, uint32 idx) const -> const String&;

		auto CompileShader(
			const StringView& shaderName,
			const Vector<const char*> codeParts,
			EShaderType shaderType
		) -> ShaderCompileResult;

	private:
		const Renderer& m_Renderer; 
		FileSystem& m_FileSystem;
		ShaderIncludeCache* m_ShaderIncludeCache;
	};
}