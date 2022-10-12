#include <DwarfRender/ShaderGraph/Expression.h>
#include <DwarfRender/ShaderGraph/Variable.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/StringOperations.h>

namespace {
	inline bool CalcIsMultiframed(const df::String& expression) {
		for (auto c : expression) {
			if (c == '#') {
				return true;
			}
		}

		return false;
	}
}

rf::sg::Expression::Expression(const Variable& var)
	: m_Expression(var.GetString()) 
	, m_Type(var.GetType())
	, m_IsMultiframed(var.IsMultiframed())
{}

rf::sg::Expression::Expression(Variable&& var)
	: m_Expression(var.GetString()) 
	, m_Type(var.GetType())
	, m_IsMultiframed(var.IsMultiframed())
{}

rf::sg::Expression::Expression(const df::String& expression, EVariableType type /*= EVariableType::Unknown*/)
	: m_Expression(expression) 
	, m_Type(type) {

	m_IsMultiframed = CalcIsMultiframed(m_Expression);
}

rf::sg::Expression::Expression(df::String&& expression, EVariableType type /*= EVariableType::Unknown*/)
	: m_Expression(std::move(expression)) 
	, m_Type(type) {
	m_IsMultiframed = CalcIsMultiframed(m_Expression);
}

rf::sg::Expression::Expression(const df::StringView& expression, EVariableType type /*= EVariableType::Unknown*/)
	: m_Expression(expression) 
	, m_Type(type) {
	m_IsMultiframed = CalcIsMultiframed(m_Expression);
}

rf::sg::Expression::Expression(double value, EVariableType type /*= EVariableType::Float*/)
	: m_Expression(std::to_string(value)) 
	, m_Type(type)
	, m_IsMultiframed(false)
{}

rf::sg::Expression::Expression(float value, EVariableType type /*= EVariableType::Float*/)
	: m_Expression(std::to_string(value)) 
	, m_Type(type)
	, m_IsMultiframed(false)
{}

rf::sg::Expression::Expression(int value, EVariableType type /*= EVariableType::Float*/)
	: m_Expression(std::to_string(value)) 
	, m_Type(type)
	, m_IsMultiframed(false)
{}

auto rf::sg::Expression::GetString() const -> const df::String& {
	return m_Expression;
}

auto rf::sg::Expression::GetType() const->EVariableType {
	return m_Type;
}

bool rf::sg::Expression::IsMultiframed() const {
	return m_IsMultiframed;
}

auto rf::sg::Expression::X() const->Expression {
	return Expression(m_Expression + ".x", EVariableType::Float);
}

auto rf::sg::Expression::Y() const->Expression {
	return Expression(m_Expression + ".y", EVariableType::Float);
}

auto rf::sg::Expression::Z() const->Expression {
	return Expression(m_Expression + ".z", EVariableType::Float);
}

auto rf::sg::Expression::W() const->Expression {
	return Expression(m_Expression + ".w", EVariableType::Float);
}

auto rf::sg::Expression::XY() const->Expression {
	return Expression(m_Expression + ".xy", EVariableType::Vec2);
}

auto rf::sg::Expression::XYZ() const->Expression {
	return Expression(m_Expression + ".xyz", EVariableType::Vec3);
}

auto rf::sg::Expression::operator [] (int frame) const -> Expression {
	DFAssert(m_IsMultiframed, "Variable is not multiframed to call frame index!");
	DFAssert(frame >= 0 && frame <= 1, "Invalid frame index!");

	return Expression(df::Replaced(m_Expression, '#', ((frame == 0) ? '0' : '1')), m_Type);
}

auto rf::sg::Expression::operator+(const Expression& other) const -> Expression {
	return Expression(df::String("(") + m_Expression + " + " + other.GetString() + df::String(")"), m_Type);
}

auto rf::sg::Expression::operator-(const Expression& other) const -> Expression {
	return Expression(df::String("(") + m_Expression + " - " + other.GetString() + df::String(")"), m_Type);
}

auto rf::sg::Expression::operator*(const Expression& other) const -> Expression {
	return Expression(df::String("(") + m_Expression + " * " + other.GetString() + df::String(")"), m_Type);
}

auto rf::sg::Expression::operator/(const Expression& other) const -> Expression {
	return Expression(df::String("(") + m_Expression + " / " + other.GetString() + df::String(")"), m_Type);
}