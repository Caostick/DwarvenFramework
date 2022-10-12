#include <DwarfRender/Loaders/PipelineCodeLoader.h>
#include <DwarfRender/Loaders/ShaderCompiler.h>
#include <DwarfRender/Renderer.h>

#include <DwarfRender/Resources/GraphicsPipelineResource.h>

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>

#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Log.h>
#include <DwarvenCore/New.h>

#include "../GraphicsPipeline.h"

#include <iostream>

rf::PipelineCodeLoader::PipelineCodeLoader(rf::Renderer& renderer)
	: m_Renderer(renderer) 
	, m_ShaderCompiler(nullptr) {

	m_ShaderCompiler = DFNew rf::ShaderCompiler;
}

rf::PipelineCodeLoader::~PipelineCodeLoader() {
	DFDelete m_ShaderCompiler;
}

auto rf::PipelineCodeLoader::MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto code = df::String((const char*)data, size);

	const df::StringView vsToken = "[Vertex Shader]";
	const df::StringView fsToken = "[Fragment Shader]";

	df::String vsCode;
	df::String fsCode;
	df::String* currentCode = nullptr;

	size_t lineIdx = 0;
	for (auto token : df::Tokenize(code, "\n")) {
		const auto line = df::Trim(token, " \t\r");

		if (line == vsToken) {
			currentCode = &vsCode;
			(*currentCode) += df::String("#line ") + std::to_string(lineIdx + 2) + "\n";
		} else if (line == fsToken) {
			currentCode = &fsCode;
			(*currentCode) += df::String("#line ") + std::to_string(lineIdx + 2) + "\n";
		} else if (currentCode) {
			(*currentCode) += df::String(line) + "\n";
		}

		lineIdx++;
	}

	rf::GraphicsPipelineResource* resource = DFNew rf::GraphicsPipelineResource;
	resource->m_GraphicsPipeline = m_Renderer.CreateGraphicsPipeline();
	auto pipeline = resource->m_GraphicsPipeline;
	pipeline->m_Name = name;

	auto& parameterSets = pipeline->m_ParameterSets;
	auto& vertexAttributes = pipeline->m_VertexAttributes;
	// Vertex Shader
	{
		const auto info = m_ShaderCompiler->CompileShader(
			name,
			vsCode.c_str(),
			EShaderType::Vertex,
			resourceManager,
			m_Renderer.GetParamSetDefinitionManager()
		);
		ProcessLog();
		
		pipeline->m_VSCode = info.m_SpirVCode;
		parameterSets = info.m_ParameterSets;
		vertexAttributes = info.m_Attributes;
	}

	// Fragment Shader
	{
		const auto info = m_ShaderCompiler->CompileShader(
			name, 
			fsCode.c_str(), 
			EShaderType::Fragment, 
			resourceManager,
			m_Renderer.GetParamSetDefinitionManager()
		);
		ProcessLog();

		pipeline->m_FSCode = info.m_SpirVCode;
		for (auto parameterSet : info.m_ParameterSets) {
			df::AddUnique(parameterSets, parameterSet);
		}
	}

	return resource;
}

void rf::PipelineCodeLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void rf::PipelineCodeLoader::UnloadResource(df::IResource* resource) const {
	rf::GraphicsPipelineResource* graphicsPipelineResource = static_cast<rf::GraphicsPipelineResource*>(resource);


	m_Renderer.DestroyGraphicsPipeline(graphicsPipelineResource->m_GraphicsPipeline);
}

auto rf::PipelineCodeLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto rf::PipelineCodeLoader::GetResourceExtension() const->df::String {
	return "gpl";
}

bool rf::PipelineCodeLoader::ProcessLog() const {
	bool result = false;

	const auto& log = m_ShaderCompiler->GetLog();
	if (!log.empty()) {
		df::Log(log.c_str());
		m_ShaderCompiler->ClearLog();
		result = true;
	}

	return result;
}