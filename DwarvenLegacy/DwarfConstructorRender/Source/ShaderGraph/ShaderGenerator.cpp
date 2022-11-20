#include <DwarfConstructorRender/ShaderGraph/ShaderGenerator.h>

#include <DwarfConstructorRender/ParamSetDefinition.h>
#include <DwarfConstructorRender/ParamSetDefinitionManager.h>

#include <DwarvenCore/Assert.h>

namespace {
	void Append(df::String& string, rf::sg::EVectorComp comp){
		if (comp == rf::sg::EVectorComp::None) {
			return;
		}

		switch (comp) {
		case rf::sg::EVectorComp::X:
			string += "x";
			break;
		case rf::sg::EVectorComp::Y:
			string += "y";
			break;
		case rf::sg::EVectorComp::Z:
			string += "z";
			break;
		case rf::sg::EVectorComp::W:
			string += "w";
			break;
		default:
			break;
		}
	}
}

rf::sg::ShaderGenerator::ShaderGenerator(const rf::ParamSetDefinitionManager& paramSetDefinitionManager)
	: m_Parameters(nullptr)
	, m_ParamSetDefinitionManager(paramSetDefinitionManager)
	, m_LocalVariableIndex(0) {
}

rf::sg::ShaderGenerator::~ShaderGenerator() {
	m_Variables.Clear();
}

void rf::sg::ShaderGenerator::Reset(const rf::ParamSetDefinition* parameters) {
	m_Parameters = parameters;

	m_LocalVariableIndex = 0;
	m_Variables.Clear();
	m_Inputs.clear();
	m_Outputs.clear();
	m_Includes.clear();
	m_CodeLines.clear();
}

void rf::sg::ShaderGenerator::RequireInclude(const df::StringView& fileName) {
	for (const auto& include : m_Includes) {
		if (include == fileName) {
			return;
		}
	}

	m_Includes.emplace_back(df::String(fileName));
}

auto rf::sg::ShaderGenerator::NewVariable(const Expression& value)->const Variable& {
	const df::String name = "Local" + std::to_string(m_LocalVariableIndex++);
	return NewVariable(name, value);
}

auto rf::sg::ShaderGenerator::NewVariable(const df::StringView& name, const Expression& value)->const Variable& {
	DFAssert(value.GetType() != EVariableType::Unknown, "Can't deduce variable type!");

	sg::Variable* variable = m_Variables.Create(*this);
	variable->m_IsMutable = false;
	variable->m_IsMultiframed = value.IsMultiframed();
	variable->m_IsInitialized = false;
	variable->m_Type = value.GetType();
	variable->m_Name = name;
	variable->m_Expression = variable->m_Name + (variable->m_IsMultiframed ? "[#]" : "");

	Init(*variable, value);

	return *variable;
}

auto rf::sg::ShaderGenerator::MakeConst(double value)->Expression {
	return Expression(
		ToString(EVariableType::Float) + "(" +
		std::to_string(value) + ")",
		EVariableType::Float
	);
}

auto rf::sg::ShaderGenerator::MakeConst(double x, double y)->Expression {
	return Expression(
		ToString(EVariableType::Vec2) + "(" +
		std::to_string(x) + ", " + 
		std::to_string(y) + ")",
		EVariableType::Vec2
	);
}

auto rf::sg::ShaderGenerator::MakeConst(double x, double y, double z)->Expression {
	return Expression(
		ToString(EVariableType::Vec3) + "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ")",
		EVariableType::Vec3
	);
}

auto rf::sg::ShaderGenerator::MakeConst(double x, double y, double z, double w)->Expression {
	return Expression(
		ToString(EVariableType::Vec4) + "(" +
		std::to_string(x) + ", " +
		std::to_string(y) + ", " +
		std::to_string(z) + ", " +
		std::to_string(w) + ")",
		EVariableType::Vec4
	);
}

auto rf::sg::ShaderGenerator::GetLocalParameter(const df::StringView& name)->const Variable& {
	const auto& parameters = *m_Parameters;

	Variable* variable = m_Variables.Create(*this);

	for (const auto& constant : parameters.GetConstants()) {
		if (constant->m_Name != name) {
			continue;
		}

		EVariableType type = EVariableType::Unknown;
		switch (constant->GetType()) {
		case rf::EShaderParameterType::Float:
			type = EVariableType::Float;
			break;
		case rf::EShaderParameterType::Vec2:
			type = EVariableType::Vec2;
			break;
		case rf::EShaderParameterType::Vec3:
			type = EVariableType::Vec3;
			break;
		case rf::EShaderParameterType::Vec4:
			type = EVariableType::Vec4;
			break;
		case rf::EShaderParameterType::Mat3:
			type = EVariableType::Mat3;
			break;
		case rf::EShaderParameterType::Mat4:
			type = EVariableType::Mat4;
			break;
		}

		variable->m_IsInitialized = true;
		variable->m_IsMultiframed = true;
		variable->m_IsMutable = false;
		variable->m_Type = type;
		variable->m_Name = name;
		variable->m_Expression = parameters.GetName() + "UBO[#]." + variable->m_Name;

		return *variable;
	}

	DFAssert(false, "Local parameter set has no required parameter!");
	return *variable;
}

auto rf::sg::ShaderGenerator::FunctionCall(EVariableType returnType, const df::StringView& name, const df::Vector<Expression>& args)->Expression {
	df::String expression = df::String(name) + "(";

	bool first = true;
	for (const auto& arg : args) {
		if (!first) {
			expression += ", ";
		}

		expression += arg.GetString();

		first = false;
	}

	expression += ")";
	return Expression(expression, returnType);
}

auto rf::sg::ShaderGenerator::ComponentMask(const Expression& variable,
	EVectorComp x,
	EVectorComp y /*= EVectorComponent::None*/,
	EVectorComp z /*= EVectorComponent::None*/,
	EVectorComp w /*= EVectorComponent::None*/)->Expression {
	df::String value = variable.GetString();
	if (x == EVectorComp::None) {
		return value;
	}
	value += ".";
	Append(value, x);
	Append(value, y);
	Append(value, z);
	Append(value, w);

	return value;
}

auto rf::sg::ShaderGenerator::Init(Variable& left, const Expression& right)->Variable& {
	DFAssert((left.GetType() == EVariableType::Unknown) || (left.GetType() == right.GetType()), "Can't assign variable. Type mismatch!");

	const df::String typeString =
		(left.IsConst() ? "const " : "") +
		ToString(left.GetType()) + " ";

	if (left.IsMultiframed()) {
		if (right.IsMultiframed()) {
			m_CodeLines.emplace_back(typeString + left.GetName() + "[2] = { " + right[0].GetString() + ", " + right[1].GetString() + " }");
		} else {
			m_CodeLines.emplace_back(typeString + left.GetName() + "[2] = { " + right.GetString() + ", " + right.GetString() + " }");
		}
	} else {
		if (right.IsMultiframed()) {
			DFAssert(false, "Can't assing multiframed to simple single frame variable!");
		} else {
			m_CodeLines.emplace_back(typeString + left.GetName() + " = " + right.GetString());
		}
	}

	return left;
}

auto rf::sg::ShaderGenerator::Assign(Variable& left, const Expression& right)->Variable& {
	DFAssert(left.IsMutable(), "Can't assing to constant!");
	DFAssert((left.GetType() == EVariableType::Unknown) || (left.GetType() == right.GetType()), "Can't assign variable. Type mismatch!");

	if (left.IsMultiframed()) {
		if (right.IsMultiframed()) {
			m_CodeLines.emplace_back(left[0].GetString() + " = " + right[0].GetString());
			m_CodeLines.emplace_back(left[1].GetString() + " = " + right[1].GetString());
		} else {
			m_CodeLines.emplace_back(left[0].GetString() + " = " + right.GetString());
			m_CodeLines.emplace_back(left[1].GetString() + " = " + right.GetString());
		}
	} else {
		if (right.IsMultiframed()) {
			DFAssert(false, "Can't assing multiframed to simple single frame variable!");
		} else {
			m_CodeLines.emplace_back(left.GetString() + " = " + right.GetString());
		}
	}

	return left;
}

auto rf::sg::ShaderGenerator::MakeCode() const->df::String {
	df::String code;
	code.reserve(1024);

	// Includes
	if (!m_Includes.empty()) {
		code += "\n";
		for (const auto& include : m_Includes) {
			code += "#include \"" + include + "\"\n";
		}
	}

	// Inputs
	if (!m_Inputs.empty()) {
		code += "\n";
		for (int i = 0; i < m_Inputs.size(); ++i) {
			if (m_Inputs[i]->IsMultiframed()) {
				code += "layout(location = " + std::to_string(i) + ") in " + ToString(m_Inputs[i]->GetType()) + " " + m_Inputs[i]->GetName() + "[2];\n";
			} else {
				code += "layout(location = " + std::to_string(i) + ") in " + ToString(m_Inputs[i]->GetType()) + " " + m_Inputs[i]->GetName() + ";\n";
			}
		}
	}

	// Outputs
	if (!m_Outputs.empty()) {
		code += "\n";
		for (int i = 0; i < m_Outputs.size(); ++i) {
			if (m_Outputs[i]->IsMultiframed()) {
				code += "layout(location = " + std::to_string(i) + ") out " + ToString(m_Outputs[i]->GetType()) + " " + m_Outputs[i]->GetName() + "[2];\n";
			} else {
				code += "layout(location = " + std::to_string(i) + ") out " + ToString(m_Outputs[i]->GetType()) + " " + m_Outputs[i]->GetName() + ";\n";
			}
		}
	}

	code += "\n";
	code += "void main() {\n";
	for (const auto& line : m_CodeLines) {
		code += "  " + line + ";\n";
	}
	code += "}";

	code.shrink_to_fit();
	return code;
}