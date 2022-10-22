#include <DwarfConstructorRender/ShaderGraph/Node.h>

#include <DwarvenCore/Assert.h>

void rf::sg::Node::AddParameter(ParamSetDeclarator& /*paramDeclarator*/) {

}

bool rf::sg::Node::IsParameter() const {
	return false;
}

bool rf::sg::Node::Set(const df::StringView& socketName, Node* node) {
	const auto& it = m_Nodes.find(df::String(socketName));
	if (it == m_Nodes.end()) {
		return false;
	}

	*(it->second) = node;

	return true;
}

bool rf::sg::Node::Set(const df::StringView& propertyName, float value) {
	const auto& it = m_FloatProperties.find(df::String(propertyName));
	if (it == m_FloatProperties.end()) {
		return false;
	}

	*(it->second) = value;

	return true;
}

bool rf::sg::Node::Set(const df::StringView& propertyName, const Vec2& value) {
	const auto& it = m_Vec2Properties.find(df::String(propertyName));
	if (it == m_Vec2Properties.end()) {
		return false;
	}

	*(it->second) = value;

	return true;
}

bool rf::sg::Node::Set(const df::StringView& propertyName, const Vec3& value) {
	const auto& it = m_Vec3Properties.find(df::String(propertyName));
	if (it == m_Vec3Properties.end()) {
		return false;
	}

	*(it->second) = value;

	return true;
}

bool rf::sg::Node::Set(const df::StringView& propertyName, const Vec4& value) {
	const auto& it = m_Vec4Properties.find(df::String(propertyName));
	if (it == m_Vec4Properties.end()) {
		return false;
	}

	*(it->second) = value;

	return true;
}

bool rf::sg::Node::Set(const df::StringView& propertyName, const df::StringView& value) {
	const auto& it = m_StringProperties.find(df::String(propertyName));
	if (it == m_StringProperties.end()) {
		return false;
	}

	*(it->second) = df::String(value);

	return true;
}

auto rf::sg::Node::GetVariable(PipelineGenerator& pipeline) -> const Variable* {
	if (!m_Variable) {
		MakeVariable(pipeline);
	}
	return m_Variable;
}

void rf::sg::Node::ResetVariable() {
	m_Variable = nullptr;
}

void rf::sg::Node::ResetValues() {
	ResetVariable();
	for (auto&& prop : m_FloatProperties) {
		*prop.second = 0.0f;
	}

	for (auto&& prop : m_Vec2Properties) {
		*prop.second = Vec2();
	}

	for (auto&& prop : m_Vec3Properties) {
		*prop.second = Vec3();
	}

	for (auto&& prop : m_Vec4Properties) {
		*prop.second = Vec4();
	}

	for (auto&& prop : m_Nodes) {
		*prop.second = nullptr;
	}

	for (auto&& prop : m_StringProperties) {
		*prop.second = "";
	}
}

void rf::sg::Node::Reset() {
	ResetVariable();

	m_FloatProperties.clear();
	m_Vec2Properties.clear();
	m_Vec3Properties.clear();
	m_Vec4Properties.clear();
	m_Nodes.clear();
}

void rf::sg::Node::Bind(const df::StringView& socketName, Node** node) {
	m_Nodes[df::String(socketName)] = node;
}

void rf::sg::Node::Bind(const df::StringView& propertyName, float* propertyPtr) {
	m_FloatProperties[df::String(propertyName)] = propertyPtr;
}

void rf::sg::Node::Bind(const df::StringView& propertyName, Vec2* propertyPtr) {
	m_Vec2Properties[df::String(propertyName)] = propertyPtr;
}

void rf::sg::Node::Bind(const df::StringView& propertyName, Vec3* propertyPtr) {
	m_Vec3Properties[df::String(propertyName)] = propertyPtr;
}

void rf::sg::Node::Bind(const df::StringView& propertyName, Vec4* propertyPtr) {
	m_Vec4Properties[df::String(propertyName)] = propertyPtr;
}

void rf::sg::Node::Bind(const df::StringView& propertyName, df::String* propertyPtr) {
	m_StringProperties[df::String(propertyName)] = propertyPtr;
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> socketNames, Node** node) {
	for (auto& name : socketNames) {
		Bind(name, node);
	}
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> propertyNames, float* propertyPtr) {
	for (auto& name : propertyNames) {
		Bind(name, propertyPtr);
	}
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> propertyNames, Vec2* propertyPtr) {
	for (auto& name : propertyNames) {
		Bind(name, propertyPtr);
	}
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> propertyNames, Vec3* propertyPtr) {
	for (auto& name : propertyNames) {
		Bind(name, propertyPtr);
	}
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> propertyNames, Vec4* propertyPtr) {
	for (auto& name : propertyNames) {
		Bind(name, propertyPtr);
	}
}

void rf::sg::Node::Bind(const df::Vector<df::StringView> propertyNames, df::String* propertyPtr) {
	for (auto& name : propertyNames) {
		Bind(name, propertyPtr);
	}
}

bool rf::sg::Node::CheckError(bool condition, const df::StringView& error) {
	if (condition && !HasError()) {
		m_Error = df::String(error);
	}

	return condition;
}

bool rf::sg::Node::HasError() const {
	return !m_Error.empty();
}