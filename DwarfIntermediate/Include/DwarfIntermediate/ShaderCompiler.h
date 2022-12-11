#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Result.h>

namespace df {
	class Renderer;
	class FileSystem;
	class ShaderIncludeCache;
	class NamedIndexedSnippetCache;

	struct PipelineCompileInfo;
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
	using ShaderCompileResult = Result<bool, String>;

	class ShaderCompiler {
	public:
		ShaderCompiler(const Renderer& renderer, FileSystem& fileSystem, const StringView& dataPath);
		~ShaderCompiler();

		void CreateVertexAttributePrototype(const StringView& name, const StringView& snippet);
		void CreateParameterSetPrototype(const StringView& name, const StringView& snippet);

		auto CompileShader(
			const StringView& shaderName,
			const StringView& code,
			EShaderType shaderType,
			PipelineCompileInfo& pipelineCompileInfo
		) -> ShaderCompileResult;

	private:
		const Renderer& m_Renderer; 
		FileSystem& m_FileSystem;
		ShaderIncludeCache* m_ShaderIncludeCache;
		NamedIndexedSnippetCache* m_VertexAttributeSnippetCache;
		NamedIndexedSnippetCache* m_ParameterSetSnippetCache;
	};
}