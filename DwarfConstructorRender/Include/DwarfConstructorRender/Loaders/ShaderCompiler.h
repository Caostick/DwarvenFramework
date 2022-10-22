#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

#include <DwarfConstructorRender/Types.h>
#include <DwarfConstructorRender/DepthState.h>
#include <DwarfConstructorRender/RasterizationState.h>
#include <DwarfConstructorRender/BlendState.h>
#include <DwarfConstructorRender/VertexAttributeRegistry.h>

namespace df {
	class ResourceManager;
}

namespace rf {
	class ParamSetDefinition;
	class ParamSetDefinitionManager;
}

namespace rf {
	enum class EShaderType {
		Vertex,
		Geometry,
		Fragment,
		Compute,
		Mesh
	};

	struct ShaderCompileInfo {
		struct VertexAttribute {
			EShaderInOutType m_Type;
			uint32 m_Location;
			df::String m_Name;
		};

		df::Vector<uint32> m_SpirVCode;
		df::Vector<const ParamSetDefinition*> m_ParameterSets;
		df::Vector<rf::VertexAttributeLocation> m_Attributes;
		rf::DepthState m_DepthState;
		rf::RasterizationState m_RasterizationState;
		rf::BlendState m_BlendState;
	};

	class ShaderCompiler {
	public:
		ShaderCompiler();
		~ShaderCompiler();

		auto CompileShader(
			const df::String& shaderName,
			const char* code,
			EShaderType shaderType,
			const df::ResourceManager& resourceManager,
			const rf::ParamSetDefinitionManager& paramSetDefinitionManager
		)->ShaderCompileInfo;

		auto GetLog() const ->const df::String&;
		void ClearLog();
	private:
		void AddLogLine(const df::StringView& line);
		void AddLogLine(const df::StringView& line, const df::StringView& arg);

		df::String m_Log;
	};
}