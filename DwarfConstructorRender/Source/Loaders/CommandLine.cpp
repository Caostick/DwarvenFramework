#include <DwarfConstructorRender/Loaders/CommandLine.h>
#include <DwarfConstructorRender/ShaderGraph/ShaderGraph.h>
#include <DwarfConstructorRender/ShaderGraph/Node.h>

#include <DwarfConstructorRender/ParamSet.h>
#include <DwarfConstructorRender/ParamSetDefinition.h>

#include <DwarvenCore/StringOperations.h>

namespace {
	const char* FirstSymbol = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* NameSymbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const char* ConstantSymbols = "0123456789.";

	bool IsValidName(const df::StringView& name) {
		if (name.empty()) {
			return false;
		}

		const df::StringView head = df::TakeLeft(name, 1);
		const df::StringView tail = df::DropLeft(name, 1);

		if (df::CountOf(head, FirstSymbol) != 1) {
			return false;
		}

		if (df::CountOf(tail, NameSymbols) != tail.length()) {
			return false;
		}

		return true;
	}

	bool IsValidFloat(const df::StringView& value) {
		if (df::CountOf(value, '.') > 1) {
			return false;
		}

		return (df::CountOf(value, ConstantSymbols) == value.length());
	}
}

auto rf::CommandLine::GetError() const -> const df::String& {
	return m_Error;
}

bool rf::CommandLine::ExecuteCode(const df::StringView& code) {
	m_Error.clear();

	m_Line = 0;
	for (const auto& line : df::Tokenize(code, "\n")) {
		m_Line++;

		const df::StringView lineTrimed = df::Trim(line, " \t\r");
		if (lineTrimed.empty()) {
			continue;
		}

		if (lineTrimed[0] == '#') {
			continue;
		}

		if (!ExecuteLine(lineTrimed)) {
			return false;
		}
	}
	return true;
}

bool rf::CommandLine::ExecuteLine(const df::StringView& line) {
	if (!ParseLine(line)) {
		return false;
	}

	return Execute();
}

bool rf::CommandLine::ParseLine(const df::StringView& line) {
	m_Left.Name = "";
	m_Left.PropertyName = "";
	m_Right.NodeType = "";
	m_Right.ConstValue = "";
	m_Right.StringValue = "";
	m_Right.NameValue = "";

	const df::StringView lineTrimed = df::Trim(line, " \t\r");

	uint32 numTokens = 0;
	df::StringView tokens[2];
	for (const auto& token : df::Tokenize(lineTrimed, "=")) {
		if (numTokens >= 2) {
			return Error("Invalid amount of line operands");
		}

		tokens[numTokens++] = df::Trim(token, " ");
	}

	if (numTokens != 2) {
		return Error("Invalid amount of line operands");
	}

	if (!ParseLeft(tokens[0])) {
		return Error("Can't parse left operand");
	}

	if (!ParseRight(tokens[1])) {
		return Error("Can't parse right operand");
	}

	return true;
}

bool rf::CommandLine::ParseLeft(const df::StringView& code) {
	if (code.empty()) {
		return Error("Left operand shouldn't be empty");
	}

	auto ptIdx = df::Find(code, '.');
	if (ptIdx != -1) {
		m_Left.Name = df::TakeLeft(code, ptIdx);
		m_Left.PropertyName = df::DropLeft(code, ptIdx + 1);

		if (!IsValidName(m_Left.PropertyName)) {
			return Error("Invalid node property name");
		}
	} else {
		m_Left.Name = code;
	}

	if (!IsValidName(m_Left.Name)) {
		return Error("Invalid node name");
	}

	return true;
}

bool rf::CommandLine::ParseRight(const df::StringView& code) {
	if (code.empty()) {
		return Error("Right operand shouldn't be empty");
	}

	if (code.front() == '[' && code.back() == ']') { // Node type
		m_Right.NodeType = df::DropRight(df::DropLeft(code, 1), 1);

		if (!IsValidName(m_Right.NodeType)) {
			return Error("Invalid node type name");
		}
	} else if (code.front() == '(' && code.back() == ')') { // Const value
		m_Right.ConstValue = df::DropRight(df::DropLeft(code, 1), 1);

		if (m_Right.ConstValue.empty()) {
			return Error("Value shouldn't be empty");
		}
	} else if (
		(code.front() == '\"' && code.back() == '\"') ||
		(code.front() == '\'' && code.back() == '\'')) { // String value

		if (code.length() < 2) {
			return Error("Invalid string token");
		}

		m_Right.StringValue = df::DropRight(df::DropLeft(code, 1), 1);
	} else { // Node name value
		m_Right.NameValue = code;

		if (!IsValidName(m_Right.NameValue)) {
			return Error("Invalid node value name");
		}
	}

	return true;
}

bool rf::CommandLine::ParseValue(float* values, uint32* count, uint32 maxCount) {
	*count = 0;

	for (auto token : df::Tokenize(m_Right.ConstValue, ",")) {
		if (*count >= maxCount) {
			return Error("Constant component count out of max count");
		}

		const df::StringView valueString = df::Trim(token, " \t");
		if (!IsValidFloat(valueString)) {
			return Error("Invalid constant component");
		}

		values[(*count)++] = std::stof(df::String(valueString));
	}

	return true;
}

bool rf::CommandLine::Error(const char* text) {
	m_Error += std::to_string(m_Line) + ":" + df::String(text) + "!\n";
	return false;
}



auto rf::CommandLine_ParamSetMaterialName::GetName() const -> const df::StringView& {
	return m_Name;
}

bool rf::CommandLine_ParamSetMaterialName::Execute() {
	if (!m_Name.empty()) {
		return true;
	}

	if (m_Left.Name != "Material") {
		return true;
	}

	if (m_Left.PropertyName != "Rule") {
		return true;
	}

	if (m_Right.StringValue.empty()) {
		return true;
	}

	m_Name = m_Right.StringValue;

	return true;
}



rf::CommandLine_ParamSet::CommandLine_ParamSet(ParamSet& paramSet) 
	: m_ParamSet(paramSet) {}

bool rf::CommandLine_ParamSet::Execute() {
	const bool setValue = !m_Left.Name.empty();

	const bool valueIsString = !m_Right.StringValue.empty();
	const bool valueIsConst = !m_Right.ConstValue.empty();

	if (!setValue) {
		return Error("Can't execute line");
	}

	if (valueIsString) {
		//auto tex = m_ParamSet.GetTextureRef(m_Left.Name);
		//if (!tex) {
		//	return Error("Texture property doesn't exist");
		//}
	}

	if (valueIsConst) {
		uint32 compCount = 0;
		float values[4] = { 0.0f };

		if (!ParseValue(values, &compCount, 4)) {
			return Error("Can't parse constant value");
		}

		if (compCount == 0) {
			return Error("Constant shouldn't be empty");
		}

		if (compCount == 1) {
			if (!m_ParamSet.SetFloatByName(m_Left.Name, float(values[0]))) {
				return Error("Float property doesn't exist");
			}
		} else if (compCount == 2) {
			if (!m_ParamSet.SetVec2ByName(m_Left.Name, Vec2(values[0], values[1]))) {
				return Error("Vec2 property doesn't exist");
			}
		} else if (compCount == 3) {
			if (!m_ParamSet.SetVec3ByName(m_Left.Name, Vec3(values[0], values[1], values[2]))) {
				return Error("Vec3 property doesn't exist");
			}
		} else if (compCount == 4) {
			if (!m_ParamSet.SetVec4ByName(m_Left.Name, Vec4(values[0], values[1], values[2], values[3]))) {
				return Error("Vec4 property doesn't exist");
			}
		}
	}

	return true;
}





rf::sg::CommandLine_ShaderGraph::CommandLine_ShaderGraph(ShaderGraph& shaderGraph) 
	: m_ShaderGraph(shaderGraph) {

	m_Nodes["output"] = m_ShaderGraph.GetOutputNode();
}

bool rf::sg::CommandLine_ShaderGraph::Execute() {
	const bool setNode = !m_Left.Name.empty() && m_Left.PropertyName.empty();
	const bool setNodeProperty = !m_Left.Name.empty() && !m_Left.PropertyName.empty();

	const bool valueIsNodeType = !m_Right.NodeType.empty();
	const bool valueIsNode = !m_Right.NameValue.empty();
	const bool valueIsString = !m_Right.StringValue.empty();
	const bool valueIsConst = !m_Right.ConstValue.empty();

	if (setNode) {
		if (!valueIsNodeType) {
			return Error("Only node type can be assigned to node name");
		}

		if (m_Nodes.find(df::String(m_Left.Name)) != m_Nodes.end()) {
			return Error("Node with such name is already defined");
		}

		auto node = m_ShaderGraph.CreateNode(m_Right.NodeType);
		if (!node) {
			return Error("Invalid node type");
		}

		m_Nodes[df::String(m_Left.Name)] = node;

		return true;
	}

	if (setNodeProperty) {
		if (m_Nodes.find(df::String(m_Left.Name)) == m_Nodes.end()) {
			return Error("Node with such name is not defined");
		}

		auto node = m_Nodes[df::String(m_Left.Name)];

		if (valueIsNode) {
			if (m_Nodes.find(df::String(m_Right.NameValue)) == m_Nodes.end()) {
				return Error("Node(value) with such name is not defined");
			}

			auto nodeValue = m_Nodes[df::String(m_Right.NameValue)];
			if (!node->Set(m_Left.PropertyName, nodeValue)) {
				return Error("Property name for node value doesn't exist");
			}
			return true;
		}

		if (valueIsString) {
			if (!node->Set(m_Left.PropertyName, m_Right.StringValue)) {
				return Error("Property name for string value doesn't exist");
			}

			return true;
		}

		if (valueIsConst) {
			uint32 compCount = 0;
			float values[4] = { 0.0f };

			if (!ParseValue(values, &compCount, 4)) {
				return Error("Can't parse constant value");
			}

			if (compCount == 0) {
				return Error("Constant shouldn't be empty");
			}

			if (compCount == 1) {
				if (!node->Set(m_Left.PropertyName, values[0])) {
					return Error("Property name for float value doesn't exist");
				}
			} else if (compCount == 2) {
				if (!node->Set(m_Left.PropertyName, Vec2(values[0], values[1]))) {
					return Error("Property name for vec2 value doesn't exist");
				}
			} else if (compCount == 3) {
				if (!node->Set(m_Left.PropertyName, Vec3(values[0], values[1], values[2]))) {
					return Error("Property name for vec3 value doesn't exist");
				}
			} else if (compCount == 4) {
				if (!node->Set(m_Left.PropertyName, Vec4(values[0], values[1], values[2], values[3]))) {
					return Error("Property name for vec4 value doesn't exist");
				}
			}
			return true;
		}
	}

	return Error("Can't execute line");
}