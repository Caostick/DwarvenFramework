#pragma once

#include <DwarfRender/ShaderGraph/ShaderGenerator.h>

namespace rf {
	namespace sg {
		class VertexShaderGenerator : public ShaderGenerator {
		public:
			VertexShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);

			auto GetVertexOutput()->Variable&;

			auto RequestOutput(EVariableType type, const df::StringView& name, const Expression& value)->const Variable&;
			auto RequestOutput(EVariableType type, const df::StringView& name, bool isMultiframed = false)->Variable&;

		protected:
			auto RequestInput(EVariableType type, const df::StringView& name, bool isMultiframed = false)->const Variable&;
		};

		template<typename SetupType>
		class TVertexShaderGenerator : public VertexShaderGenerator {
		public:
			TVertexShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);

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

			template<typename AttributeType>
			auto RequestVertexAttribute()->const Variable&;
		};
	}
}

#include <DwarfRender/ShaderGraph/VertexShaderGenerator.inl>