#pragma once

#include <DwarfConstructorRender/ShaderGraph/Types.h>
#include <DwarfConstructorRender/ShaderGraph/Expression.h>
#include <DwarfConstructorRender/ShaderGraph/ShaderGenerator.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

namespace rf {
	class ParamSetDefinition;

	namespace sg {
		class ShaderGraph;
		class Variable;
	}
}

namespace rf {
	namespace sg {
		class PipelineGenerator {
		public:
			PipelineGenerator();
			virtual ~PipelineGenerator() {}

			bool IsMultiframed() const;
			auto NewVariable(const Expression& value)->const Variable&;

			//auto GetGlobalParameter(const df::StringView& name)->const Variable&;
			//auto GetViewParameter(const df::StringView& name)->const Variable&;
			//auto GetStageParameter(const df::StringView& stageName, const df::StringView& name)->const Variable&;
			auto GetLocalParameter(const df::StringView& name)->const Variable&;

			auto MakeConstant(double value) -> const sg::Variable&;
			auto MakeConstant(double x, double y) -> const sg::Variable&;
			auto MakeConstant(double x, double y, double z) -> const sg::Variable&;
			auto MakeConstant(double x, double y, double z, double w) -> const sg::Variable&;

			auto FunctionCall(EVariableType returnType, const df::StringView& name, const df::Vector<Expression>& args)->Expression;
			auto ComponentMask(const Expression& variable,
				EVectorComp x,
				EVectorComp y = EVectorComp::None,
				EVectorComp z = EVectorComp::None,
				EVectorComp w = EVectorComp::None
			)->Expression;

		protected:
			ShaderGenerator* m_CurrentShader;
			bool m_IsMultiframed;
		};
	}
}