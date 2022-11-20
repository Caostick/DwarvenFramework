#include <DwarfConstructorRender/ShaderGraph/Variable.h>

#include <DwarfConstructorRender/ShaderGraph/ShaderGenerator.h>

#include <DwarvenCore/StringOperations.h>

rf::sg::Variable::Variable(ShaderGenerator& shader) 
	: m_Shader(shader) {

}

auto rf::sg::Variable::GetType() const ->EVariableType {
	return m_Type;
}

auto rf::sg::Variable::GetName() const -> const df::String& {
	return m_Name;
}

auto rf::sg::Variable::GetString() const -> const df::String& {
	return m_Expression;
}

bool rf::sg::Variable::IsConst() const {
	return !m_IsMutable;
}

bool rf::sg::Variable::IsMutable() const {
	return m_IsMutable;
}

bool rf::sg::Variable::IsMultiframed() const {
	return m_IsMultiframed;
}

bool rf::sg::Variable::IsInitialized() const {
	return m_IsInitialized;
}

auto rf::sg::Variable::X() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec2 ||
		m_Type == EVariableType::Vec3 ||
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".x", EVariableType::Float);
}

auto rf::sg::Variable::Y() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec2 ||
		m_Type == EVariableType::Vec3 ||
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".y", EVariableType::Float);
}

auto rf::sg::Variable::Z() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec3 ||
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".z", EVariableType::Float);
}

auto rf::sg::Variable::W() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".w", EVariableType::Float);
}

auto rf::sg::Variable::XY() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec3 ||
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".xy", EVariableType::Vec2);
}

auto rf::sg::Variable::XYZ() const->Expression {
	DFAssert(
		m_Type == EVariableType::Vec4,
		"Invalid type for swizzle!"
	);

	return Expression(m_Expression + ".xyz", EVariableType::Vec3);
}

auto rf::sg::Variable::operator [] (int frame) const -> Expression {
	DFAssert(m_IsMultiframed, "Variable is not multiframed to call frame index!");
	DFAssert(frame >= 0 && frame <= 1, "Invalid frame index!");

	return Expression(df::Replaced(m_Expression, '#', ((frame == 0) ? '0' : '1')), m_Type);
}

auto rf::sg::Variable::operator = (const Variable& other)->Variable& {
	if (m_IsInitialized) {
		m_Shader.Assign(*this, other);
	} else {
		m_Shader.Init(*this, other);
		m_IsInitialized = true;
	}

	return *this;
}

auto rf::sg::Variable::operator = (const Expression& other)->Variable& {
	if (m_IsInitialized) {
		m_Shader.Assign(*this, other);
	} else {
		m_Shader.Init(*this, other);
		m_IsInitialized = true;
	}

	return *this;
}





auto operator-(const rf::sg::Expression& variable)->rf::sg::Expression {
	return rf::sg::Expression("-" + variable.GetString(), variable.GetType());
}

auto operator+(const rf::sg::Variable& left, const rf::sg::Variable& right) ->rf::sg::Expression {
	return rf::sg::Expression(left) + rf::sg::Expression(right);
}

auto operator+(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression {
	return rf::sg::Expression(left) + right;
}

auto operator-(const rf::sg::Variable& left, const rf::sg::Variable& right) ->rf::sg::Expression {
	return rf::sg::Expression(left) - rf::sg::Expression(right);
}

auto operator-(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression {
	return rf::sg::Expression(left) - right;
}

auto operator*(const rf::sg::Variable& left, const rf::sg::Variable& right) ->rf::sg::Expression {
	return rf::sg::Expression(left) * rf::sg::Expression(right);
}

auto operator*(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression {
	return rf::sg::Expression(left) * right;
}

auto operator/(const rf::sg::Variable& left, const rf::sg::Variable& right) ->rf::sg::Expression {
	return rf::sg::Expression(left) / rf::sg::Expression(right);
}

auto operator/(const rf::sg::Variable& left, const rf::sg::Expression& right)->rf::sg::Expression {
	return rf::sg::Expression(left) / right;
}