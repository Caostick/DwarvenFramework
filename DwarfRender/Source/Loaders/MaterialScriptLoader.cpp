#include <DwarfRender/Loaders/MaterialScriptLoader.h>
#include <DwarfRender/Loaders/CommandLine.h>

#include <DwarfRender/Renderer.h>
#include <DwarfRender/MaterialRule.h>
#include <DwarfRender/Material.h>

#include <DwarfRender/Resources/MaterialResource.h>
#include <DwarfRender/Resources/MaterialRuleResource.h>

#include <DwarfRender/ShaderGraph/ShaderGraph.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Log.h>
#include <DwarvenCore/StringOperations.h>

#include <DwarfResources/ResourceManager.h>

rf::MaterialScriptLoader::MaterialScriptLoader(rf::Renderer& renderer)
	: m_Renderer(renderer) {}

auto rf::MaterialScriptLoader::MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* {
	const auto materialCode = df::String((const char*)data, size);

	auto resource = DFNew rf::MaterialResource;

	uint32 numParts = 0;
	df::StringView scriptParts[2];
	for (auto part : df::Tokenize(materialCode, "+")) {
		if (numParts >= 2) {
			df::log::WarningFmt("Invalid script for material(%s)!", name.c_str());
			return resource;
		}

		scriptParts[numParts++] = part;
	}

	rf::CommandLine_ParamSetMaterialName cmdMatName;
	if (!cmdMatName.ExecuteCode(scriptParts[0])) {
		df::log::WarningFmt("Script parsing failed for material(%s):\n%s", 
			name.c_str(), 
			cmdMatName.GetError().c_str()
		);

		return resource;
	}

	if (cmdMatName.GetName().empty()) {
		df::log::WarningFmt("Can't get material rule name for material(%s)!", name.c_str());
		return resource;
	}

	auto materialRuleResource = resourceManager.GetResource<rf::MaterialRuleResource>(cmdMatName.GetName());
	if (!materialRuleResource) {
		df::log::WarningFmt("Can't get material rule(%s) for material(%s)!",
			df::String(cmdMatName.GetName()).c_str(),
			name.c_str()
		);
		return resource;
	}

	rf::MaterialRuleId materualRule = materialRuleResource->m_MaterialRule;

	resource->m_Material = m_Renderer.CreateMaterial(materualRule);

	rf::CommandLine_ParamSet cmd(*resource->m_Material->m_Params);
	if (!cmd.ExecuteCode(scriptParts[1])) {
		df::log::WarningFmt("Failed to parse material(%s) parameters:\n%s",
			name.c_str(),
			cmd.GetError().c_str()
		);

		return resource;
	}

	return resource;
}

void rf::MaterialScriptLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void rf::MaterialScriptLoader::UnloadResource(df::IResource* resource) const {
	rf::MaterialResource* materialResource = static_cast<rf::MaterialResource*>(resource);
	if (materialResource->m_Material) {
		m_Renderer.DestroyMaterial(materialResource->m_Material);
	}
}

auto rf::MaterialScriptLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto rf::MaterialScriptLoader::GetResourceExtension() const->df::String {
	return "mtl";
}