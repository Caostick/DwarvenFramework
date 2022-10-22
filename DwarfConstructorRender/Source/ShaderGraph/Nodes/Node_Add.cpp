#include <DwarfConstructorRender/ShaderGraph/Nodes/Node_Add.h>
#include <DwarfConstructorRender/ShaderGraph/PipelineGenerator.h>

void rf::sg::Node_Add::MakeBindings() {
	Bind({ "A", "Left" }, &m_A);
	Bind({ "B", "Right" }, &m_B);
}

void rf::sg::Node_Add::MakeVariable(PipelineGenerator& pipeline) {
	const Variable* a = m_A ? m_A->GetVariable(pipeline) : nullptr;
	if (CheckError(!a, "A is not set!")) {
		return;
	}

	const Variable* b = m_B ? m_B->GetVariable(pipeline) : nullptr;
	if (CheckError(!b, "B is not set!")) {
		return;
	}

	const bool typesAreMatch = 
		(a->GetType() == b->GetType()) ||
		(a->GetType() == EVariableType::Float) ||
		(b->GetType() == EVariableType::Float);

	if (CheckError(!typesAreMatch, "A and B have different types!")) {
		return;
	}

	m_Variable = &pipeline.NewVariable(*a + *b);
}