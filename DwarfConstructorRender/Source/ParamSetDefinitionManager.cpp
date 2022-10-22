#include <DwarfConstructorRender/ParamSetDefinitionManager.h>
#include <DwarfConstructorRender/ParamSetDefinition.h>

#include <DwarvenCore/New.h>

#include "DescriptorSetDefinitions.h"

rf::ParamSetDefinitionManager::~ParamSetDefinitionManager() {
	DFDelete m_Global;
	DFDelete m_View;

	for (auto&& psd : m_Passes) {
		DFDelete psd.second;
	}

	for (auto&& psd : m_Materials) {
		DFDelete psd.second;
	}
}

void rf::ParamSetDefinitionManager::DestroyRenderData(rf::RenderCore& renderCore) {
	m_Global->DestroyRenderData(renderCore);
	m_View->DestroyRenderData(renderCore);

	for (auto&& psd : m_Passes) {
		psd.second->DestroyRenderData(renderCore);
	}

	for (auto&& psd : m_Materials) {
		psd.second->DestroyRenderData(renderCore);
	}
}

auto rf::ParamSetDefinitionManager::RequestGlobalDefinition()->rf::ParamSetDefinition* {
	if (!m_Global) {
		m_Global = DFNew rf::ParamSetDefinition("Global", rf::DS_Global);
	}
	return m_Global;
}

auto rf::ParamSetDefinitionManager::GetGlobalDefinition() const -> const rf::ParamSetDefinition* {
	return m_Global;
}

auto rf::ParamSetDefinitionManager::RequestViewDefinition()->rf::ParamSetDefinition* {
	if (!m_View) {
		m_View = DFNew rf::ParamSetDefinition("View", rf::DS_View);
	}
	return m_View;
}

auto rf::ParamSetDefinitionManager::GetViewDefinition() const -> const rf::ParamSetDefinition* {
	return m_View;
}

auto rf::ParamSetDefinitionManager::RequestDefinitionForPass(const df::StringView& passName)->rf::ParamSetDefinition* {
	auto it = m_Passes.find(df::String(passName));
	if (it != m_Passes.end()) {
		return it->second;
	}

	auto def = DFNew rf::ParamSetDefinition(passName, rf::DS_Stage);
	m_Passes[df::String(passName)] = def;
	return def;
}

auto rf::ParamSetDefinitionManager::GetDefinitionForPass(const df::StringView& passName) const -> const rf::ParamSetDefinition* {
	auto it = m_Passes.find(df::String(passName));
	if (it == m_Passes.end()) {
		return nullptr;
	}
	return it->second;
}

auto rf::ParamSetDefinitionManager::RequestDefinitionForMaterial(const df::StringView& materialName)->rf::ParamSetDefinition* {
	auto it = m_Materials.find(df::String(materialName));
	if (it != m_Materials.end()) {
		return it->second;
	}

	auto def = DFNew rf::ParamSetDefinition(materialName, rf::DS_Material);
	m_Materials[df::String(materialName)] = def;
	return def;
}

auto rf::ParamSetDefinitionManager::GetDefinitionForMaterial(const df::StringView& materialName) const -> const rf::ParamSetDefinition* {
	auto it = m_Materials.find(df::String(materialName));
	if (it == m_Materials.end()) {
		return nullptr;
	}
	return it->second;
}