#include <DwarfRender/Loaders/ShaderCodeLoader.h>
#include <DwarfRender/Loaders/ShaderCompiler.h>
#include <DwarfRender/Renderer.h>

#include <DwarfRender/Resources/ShaderResource.h>

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>

#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Log.h>
#include <DwarvenCore/New.h>

#include "../ShaderModule.h"

#include <iostream>

rf::ShaderCodeLoader::ShaderCodeLoader(rf::Renderer& renderer)
	: m_Renderer(renderer)
	, m_ShaderCompiler(nullptr) {

	m_ShaderCompiler = DFNew rf::ShaderCompiler;
}

rf::ShaderCodeLoader::~ShaderCodeLoader() {
	DFDelete m_ShaderCompiler;
}

void rf::ShaderCodeLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {
}

auto rf::ShaderCodeLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto rf::ShaderCodeLoader::LoadShader(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size)  const -> rf::ShaderCompileInfo {
	const auto code = df::String((const char*)data, size);
	const auto shaderType = GetShaderType();

	const auto info = m_ShaderCompiler->CompileShader(
		name,
		code.c_str(),
		shaderType,
		resourceManager,
		m_Renderer.GetParamSetDefinitionManager()
	);
	ProcessLog();

	return info;
}

bool rf::ShaderCodeLoader::ProcessLog() const {
	bool result = false;

	const auto& log = m_ShaderCompiler->GetLog();
	if (!log.empty()) {
		df::Log(log.c_str());
		m_ShaderCompiler->ClearLog();
		result = true;
	}

	return result;
}


rf::VertexShaderCodeLoader::VertexShaderCodeLoader(rf::Renderer& renderer) 
	: rf::ShaderCodeLoader(renderer) {}

auto rf::VertexShaderCodeLoader::MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto shaderInfo = LoadShader(resourceManager, name, data, size);

	rf::VertexShaderResource* resource = DFNew rf::VertexShaderResource;
	resource->m_ShaderModuleId = m_Renderer.CreateVertexShaderModule(name, shaderInfo.m_SpirVCode);

	resource->m_ShaderModuleId->m_Attributes = shaderInfo.m_Attributes;
	resource->m_ShaderModuleId->m_RasterizationState = shaderInfo.m_RasterizationState;
	resource->m_ShaderModuleId->m_DepthState = shaderInfo.m_DepthState;
	resource->m_ShaderModuleId->m_ParameterSets = shaderInfo.m_ParameterSets;

	return resource;
}

void rf::VertexShaderCodeLoader::UnloadResource(df::IResource* resource) const {
	rf::VertexShaderResource* shaderResource = static_cast<rf::VertexShaderResource*>(resource);

	m_Renderer.DestroyVertexShaderModule(shaderResource->m_ShaderModuleId);
}

auto rf::VertexShaderCodeLoader::GetResourceExtension() const->df::String {
	return "vsh";
}

auto rf::VertexShaderCodeLoader::GetShaderType() const -> rf::EShaderType {
	return rf::EShaderType::Vertex;
}



rf::FragmentShaderCodeLoader::FragmentShaderCodeLoader(rf::Renderer& renderer)
	: rf::ShaderCodeLoader(renderer) {}

auto rf::FragmentShaderCodeLoader::MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto shaderInfo = LoadShader(resourceManager, name, data, size);

	rf::FragmentShaderResource* resource = DFNew rf::FragmentShaderResource;
	resource->m_ShaderModuleId = m_Renderer.CreateFragmentShaderModule(name, shaderInfo.m_SpirVCode);

	resource->m_ShaderModuleId->m_ParameterSets = shaderInfo.m_ParameterSets;
	resource->m_ShaderModuleId->m_BlendState = shaderInfo.m_BlendState;

	return resource;
}

void rf::FragmentShaderCodeLoader::UnloadResource(df::IResource* resource) const {
	rf::FragmentShaderResource* shaderResource = static_cast<rf::FragmentShaderResource*>(resource);

	m_Renderer.DestroyFragmentShaderModule(shaderResource->m_ShaderModuleId);
}

auto rf::FragmentShaderCodeLoader::GetResourceExtension() const->df::String {
	return "fsh";
}

auto rf::FragmentShaderCodeLoader::GetShaderType() const -> rf::EShaderType {
	return rf::EShaderType::Fragment;
}



rf::ComputeShaderCodeLoader::ComputeShaderCodeLoader(rf::Renderer& renderer)
	: rf::ShaderCodeLoader(renderer) {}

auto rf::ComputeShaderCodeLoader::MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto shaderInfo = LoadShader(resourceManager, name, data, size);

	rf::ComputeShaderResource* resource = DFNew rf::ComputeShaderResource;
	resource->m_ShaderModuleId = m_Renderer.CreateComputeShaderModule(name, shaderInfo.m_SpirVCode);
	resource->m_ShaderModuleId->m_ParameterSets = shaderInfo.m_ParameterSets;

	return resource;
}

void rf::ComputeShaderCodeLoader::UnloadResource(df::IResource* resource) const {
	rf::ComputeShaderResource* shaderResource = static_cast<rf::ComputeShaderResource*>(resource);

	m_Renderer.DestroyComputeShaderModule(shaderResource->m_ShaderModuleId);
}

auto rf::ComputeShaderCodeLoader::GetResourceExtension() const->df::String {
	return "csh";
}

auto rf::ComputeShaderCodeLoader::GetShaderType() const -> rf::EShaderType {
	return rf::EShaderType::Compute;
}