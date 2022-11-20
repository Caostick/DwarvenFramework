#pragma once

#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

namespace rf {
	class ParamSetDefinition;
	class RenderCore;
}

namespace rf {
	class ParamSetDefinitionManager {
	public:
		ParamSetDefinitionManager() = default;
		~ParamSetDefinitionManager();

		void DestroyRenderData(rf::RenderCore& renderCore);

		auto RequestGlobalDefinition()->rf::ParamSetDefinition*;
		auto GetGlobalDefinition()const -> const rf::ParamSetDefinition*;

		auto RequestViewDefinition()->rf::ParamSetDefinition*;
		auto GetViewDefinition()const -> const rf::ParamSetDefinition*;

		auto RequestDefinitionForPass(const df::StringView& passName)->rf::ParamSetDefinition*;
		auto GetDefinitionForPass(const df::StringView& passName)const -> const rf::ParamSetDefinition*;

		auto RequestDefinitionForMaterial(const df::StringView& materialName)->rf::ParamSetDefinition*;
		auto GetDefinitionForMaterial(const df::StringView& materialName)const -> const rf::ParamSetDefinition*;

	private:
		rf::ParamSetDefinition* m_Global = nullptr;
		rf::ParamSetDefinition* m_View = nullptr;

		df::HashMap<df::String, rf::ParamSetDefinition*> m_Passes;
		df::HashMap<df::String, rf::ParamSetDefinition*> m_Materials;
	};
}