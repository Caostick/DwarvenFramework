#pragma once

#include <DwarfRender/ShaderGraph/Types.h>
#include <DwarfRender/ShaderGraph/Expression.h>

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>

namespace rf {
	namespace sg {
		class ShaderGenerator;
		class VertexShaderGenerator;
		class FragmentShaderGenerator;

		template<typename>
		struct TParameterVariableGenerator;
	}
}

namespace rf {
	namespace sg {
		class Variable {
			friend class sg::Expression;
			friend class sg::ShaderGenerator;
			friend class sg::VertexShaderGenerator;
			friend class sg::FragmentShaderGenerator;

			template<typename>
			friend struct sg::TParameterVariableGenerator;
		public:
			Variable(ShaderGenerator& shader);

			auto GetType() const->EVariableType;
			auto GetName() const -> const df::String&;
			auto GetString() const -> const df::String&;
			bool IsConst() const;
			bool IsMutable() const;
			bool IsMultiframed() const;
			bool IsInitialized() const;

			auto X() const->Expression;
			auto Y() const->Expression;
			auto Z() const->Expression;
			auto W() const->Expression;
			auto XY() const->Expression;
			auto XYZ() const->Expression;

			auto operator [] (int frame) const -> Expression;

			auto operator = (const Variable& other)->Variable&;
			auto operator = (const Expression& other)->Variable&;

		private:
			ShaderGenerator& m_Shader;

			bool m_IsMutable;
			bool m_IsMultiframed;
			bool m_IsInitialized;
			EVariableType m_Type;
			df::String m_Name;
			df::String m_Expression;
		};
	}
}

auto operator-(const rf::sg::Expression& variable)->rf::sg::Expression;

auto operator+(const rf::sg::Variable& left, const rf::sg::Variable& right)->rf::sg::Expression;
auto operator+(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression;
auto operator-(const rf::sg::Variable& left, const rf::sg::Variable& right)->rf::sg::Expression;
auto operator-(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression;
auto operator*(const rf::sg::Variable& left, const rf::sg::Variable& right)->rf::sg::Expression;
auto operator*(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression;
auto operator/(const rf::sg::Variable& left, const rf::sg::Variable& right)->rf::sg::Expression;
auto operator/(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression;