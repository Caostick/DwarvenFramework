#include <DwarfRender/Loaders/MaterialRuleScriptLoader.h>
#include <DwarfRender/Loaders/CommandLine.h>

#include <DwarfRender/Resources/MaterialRuleResource.h>
#include <DwarfRender/Renderer.h>
#include <DwarfRender/ParamSetDefinition.h>
#include <DwarfRender/MaterialRule.h>
#include <DwarfRender/Material.h>

#include <DwarfRender/ShaderGraph/ShaderGraph.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Log.h>

rf::MaterialRuleScriptLoader::MaterialRuleScriptLoader(rf::Renderer& renderer) 
	: m_Renderer(renderer) {
}

auto rf::MaterialRuleScriptLoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto materialCode = df::String((const char*)data, size);

	auto resource = DFNew rf::MaterialRuleResource;
	resource->m_MaterialRule = m_Renderer.CreateMaterialRule(name);
	resource->m_MaterialRule->m_ShaderGraph = m_Renderer.CreateShaderGraph();

	auto params = resource->m_MaterialRule->m_ParametersDefinition;
	auto shaderGraph = resource->m_MaterialRule->m_ShaderGraph;

	if (!shaderGraph) {
		df::log::WarningFmt("Can't create shader graph for material rule (%s)", name.c_str());
		return resource;
	}

	rf::sg::CommandLine_ShaderGraph cmd(*shaderGraph);
	if (!cmd.ExecuteCode(materialCode)) {
		df::log::WarningFmt("Shader graph for material rule (%s) failed: \n%s", name.c_str(), cmd.GetError().c_str());
		shaderGraph->CreateDefault();
	}

	shaderGraph->MakeParamSetDefinition(*params);
	params->Build();

	m_Renderer.InitMaterialRule(resource->m_MaterialRule);
	resource->m_MaterialRule->m_DefaultMaterial = m_Renderer.CreateMaterial(resource->m_MaterialRule);

	return resource;
}

void rf::MaterialRuleScriptLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void rf::MaterialRuleScriptLoader::UnloadResource(df::IResource* resource) const {
	rf::MaterialRuleResource* materialRuleResource = static_cast<rf::MaterialRuleResource*>(resource);
	m_Renderer.DestroyMaterial(materialRuleResource->m_MaterialRule->m_DefaultMaterial);
	m_Renderer.DestroyMaterialRule(materialRuleResource->m_MaterialRule);

	materialRuleResource->m_MaterialRule->m_ShaderGraph->Release();
	DFDelete materialRuleResource->m_MaterialRule->m_ShaderGraph;
}

auto rf::MaterialRuleScriptLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto rf::MaterialRuleScriptLoader::GetResourceExtension() const->df::String {
	return "mtlr";
}