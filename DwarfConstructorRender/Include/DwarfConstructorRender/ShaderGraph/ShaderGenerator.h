#pragma once

#include <DwarfConstructorRender/ShaderGraph/Variable.h>
#include <DwarfConstructorRender/ShaderGraph/Expression.h>

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/ObjectPool.h>

namespace rf {
	class ParamSetDefinition;
	class ParamSetDefinitionManager;

	template<typename>
	struct TParameterVariableGenerator;
}

namespace rf {
	namespace sg {
		class ShaderGenerator {
			template<typename>
			friend struct TParameterVariableGenerator;
		public:
			ShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager);
			~ShaderGenerator();

			void Reset(const rf::ParamSetDefinition* parameters);

			void RequireInclude(const df::StringView& fileName);

			auto NewVariable(const Expression& value)->const Variable&;
			auto NewVariable(const df::StringView& name, const Expression& value)->const Variable&;

			auto MakeConst(double value)->Expression;
			auto MakeConst(double x, double y)->Expression;
			auto MakeConst(double x, double y, double z)->Expression;
			auto MakeConst(double x, double y, double z, double w)->Expression;

			auto GetLocalParameter(const df::StringView& name)->const Variable&;

			auto FunctionCall(EVariableType returnType, const df::StringView& name, const df::Vector<Expression>& args)->Expression;

			auto ComponentMask(const Expression& variable,
				EVectorComp x,
				EVectorComp y = EVectorComp::None,
				EVectorComp z = EVectorComp::None,
				EVectorComp w = EVectorComp::None
			)->Expression;

			auto Init(Variable& left, const Expression& right)->Variable&;
			auto Assign(Variable& left, const Expression& right)->Variable&;

			auto MakeCode() const->df::String;

		protected:
			const rf::ParamSetDefinitionManager& m_ParamSetDefinitionManager;
			const rf::ParamSetDefinition* m_Parameters;

			uint32 m_LocalVariableIndex;

			df::ObjectPool<Variable> m_Variables;

			df::Vector<const Variable*> m_Inputs;
			df::Vector<Variable*> m_Outputs;

			df::Vector<df::String> m_Includes;
			df::Vector<df::String> m_CodeLines;
		};

		template<typename SetupType>
		struct TParameterVariableGenerator {
			template<typename ParamType>
			static auto GetGlobalParameter(ShaderGenerator* shaderGenerator)->const Variable&;

			template<typename ParamType>
			static auto GetViewParameter(ShaderGenerator* shaderGenerator)->const Variable&;

			template<typename RenderPassType, typename ParamType>
			static auto GetPassParameter(ShaderGenerator* shaderGenerator)->const Variable&;

			template<typename ParamType>
			static auto GetGlobalInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul = 1, uint32 add = 0)->const Variable&;

			template<typename ParamType>
			static auto GetViewInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul = 1, uint32 add = 0)->const Variable&;

			template<typename RenderPassType, typename ParamType>
			static auto GetPassInstanceBufferValue(ShaderGenerator* shaderGenerator, uint32 mul = 1, uint32 add = 0)->const Variable&;

		protected:
			template<typename ParamType>
			static auto CreateVariable(ShaderGenerator* shaderGenerator, bool isMultiframed)->Variable&;
		};
	}
}

#include <DwarfConstructorRender/ShaderGraph/ShaderGenerator.inl>