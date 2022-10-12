#pragma once

#include <DwarfRender/ShaderGraph/ShaderGenerator.h>

namespace rf {
	namespace sg {
		class FragmentShaderGenerator : public ShaderGenerator {
		public:
			FragmentShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);

			auto RequestInput(EVariableType type, const df::StringView& name, bool isMultiframed = false)->const Variable&;

		protected:
			auto RequestOutput(EVariableType type, const df::StringView& name)->Variable&;
		};

		template<typename SetupType>
		class TFragmentShaderGenerator : public FragmentShaderGenerator {
		public:
			TFragmentShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);

			template<typename ParamType>
			auto GetGlobalParameter()->const Variable&;

			template<typename ParamType>
			auto GetViewParameter()->const Variable&;

			template<typename RenderPassType, typename ParamType>
			auto GetPassParameter()->const Variable&;

			template<typename ParamType>
			auto GetGlobalInstanceBufferValue(uint32 mul = 1, uint32 add = 0)->const Variable&;

			template<typename ParamType>
			auto GetViewInstanceBufferValue(uint32 mul = 1, uint32 add = 0)->const Variable&;

			template<typename RenderPassType, typename ParamType>
			auto GetPassInstanceBufferValue(uint32 mul = 1, uint32 add = 0)->const Variable&;

			template<typename RenderPassType, typename RenderTargetType>
			auto RequestOutput()->Variable&;
		};
	}
}

#include <DwarfRender/ShaderGraph/FragmentShaderGenerator.inl>