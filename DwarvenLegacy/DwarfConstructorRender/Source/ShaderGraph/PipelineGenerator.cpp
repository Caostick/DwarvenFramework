#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>
#include <DwarfConstructorRender/ShaderGraph/Variable.h>

#include <DwarfConstructorRender/ParamSetDefinition.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

rf::sg::PipelineGenerator::PipelineGenerator()
	: m_IsMultiframed(false)
	, m_CurrentShader(nullptr) {
}

bool rf::sg::PipelineGenerator::IsMultiframed() const {
	return m_IsMultiframed;
}

auto rf::sg::PipelineGenerator::NewVariable(const Expression& value)->const Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->NewVariable(value);
}

//auto rf::sg::PipelineGenerator::GetGlobalParameter(const df::StringView& name)->const Variable& {
//	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");
//
//	return m_CurrentShader->GetGlobalParameter(name);
//}
//
//auto rf::sg::PipelineGenerator::GetViewParameter(const df::StringView& name)->const Variable& {
//	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");
//
//	return m_CurrentShader->GetViewParameter(name);
//}
//
//auto rf::sg::PipelineGenerator::GetStageParameter(const df::StringView& stageName, const df::StringView& name)->const Variable& {
//	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");
//
//	return m_CurrentShader->GetPassParameter(stageName, name);
//}

auto rf::sg::PipelineGenerator::GetLocalParameter(const df::StringView& name)->const Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->GetLocalParameter(name);
}

auto rf::sg::PipelineGenerator::MakeConstant(double value) -> const sg::Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->NewVariable(m_CurrentShader->MakeConst(value));
}

auto rf::sg::PipelineGenerator::MakeConstant(double x, double y) -> const sg::Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->NewVariable(m_CurrentShader->MakeConst(x, y));
}

auto rf::sg::PipelineGenerator::MakeConstant(double x, double y, double z) -> const sg::Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->NewVariable(m_CurrentShader->MakeConst(x, y, z));
}

auto rf::sg::PipelineGenerator::MakeConstant(double x, double y, double z, double w) -> const sg::Variable& {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->NewVariable(m_CurrentShader->MakeConst(x, y, z, w));
}

auto rf::sg::PipelineGenerator::FunctionCall(
	EVariableType returnType,
	const df::StringView& name,
	const df::Vector<Expression>& args)->Expression {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->FunctionCall(returnType, name, args);
}

auto rf::sg::PipelineGenerator::ComponentMask(const Expression& variable,
	EVectorComp x,
	EVectorComp y /*= EVectorComp::None*/,
	EVectorComp z /*= EVectorComp::None*/,
	EVectorComp w /*= EVectorComp::None*/)->Expression {
	DFAssert(m_CurrentShader != nullptr, "Shader is not set!");

	return m_CurrentShader->ComponentMask(variable, x, y, z, w);
}