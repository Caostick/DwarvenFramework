#pragma once

#include <DwarfConstructorRender/ShaderGraph/Types.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>

namespace rf {
	namespace sg {
		class Variable;
	}
}

namespace rf {
	namespace sg {
		class Expression {
		public:
			Expression(const Variable& var);
			Expression(Variable&& var);
			Expression(const df::String& expression, EVariableType type = EVariableType::Unknown);
			Expression(df::String&& expression, EVariableType type = EVariableType::Unknown);
			Expression(const df::StringView& expression, EVariableType type = EVariableType::Unknown);
			Expression(double value, EVariableType type = EVariableType::Float);
			Expression(float value, EVariableType type = EVariableType::Float);
			Expression(int value, EVariableType type = EVariableType::Float);

			auto GetString() const -> const df::String&;
			auto GetType() const->EVariableType;
			bool IsMultiframed() const;

			auto X() const->Expression;
			auto Y() const->Expression;
			auto Z() const->Expression;
			auto W() const->Expression;
			auto XY() const->Expression;
			auto XYZ() const->Expression;

			auto operator [] (int frame) const->Expression;

			auto operator = (const Variable& other) = delete;
			auto operator = (const Expression& other) = delete;

			auto operator + (const Expression& other) const->Expression;
			auto operator - (const Expression& other) const->Expression;
			auto operator * (const Expression& other) const->Expression;
			auto operator / (const Expression& other) const->Expression;

		private:
			df::String m_Expression;
			EVariableType m_Type;
			bool m_IsMultiframed;
		};
	}
}