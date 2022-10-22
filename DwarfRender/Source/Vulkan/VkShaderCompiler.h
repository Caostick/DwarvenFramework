#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#include "VkShaderCompileInfo.h"

namespace vk {
	class RenderCore;
	class ParamSetDefinition;
	class ParamSetDefinitionManager;
}

namespace vk {
	enum class EShaderType {
		Vertex,
		Geometry,
		Fragment,
		Compute,
		Mesh
	};

	class ShaderCompiler {
	public:
		ShaderCompiler(vk::RenderCore& renderCore);
		~ShaderCompiler();

		auto CompileShader(
			const df::String& shaderName,
			const char* code,
			EShaderType shaderType
		)->ShaderCompileInfo;

		auto GetLog() const ->const df::String&;
		void ClearLog();

	private:
		void AddLogLine(const df::StringView& line);
		void AddLogLine(const df::StringView& line, const df::StringView& arg);

	private:
		vk::RenderCore& m_RenderCore;

		df::String m_Log;
	};
}