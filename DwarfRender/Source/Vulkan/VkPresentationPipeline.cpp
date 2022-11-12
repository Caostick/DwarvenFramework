#include "VkPresentationPipeline.h"
#include "VkRenderCore.h"
#include "VkParameterSet.h"
#include "VkGraphicsPipeline.h"

bool vk::PresentationPipeline::Init(vk::RenderCore& renderCore) {
	const char* vsCode =
		"layout(location = 0) out vec2 outTexcoord;\n"
		"\n"
		"vec2 positions[3] = vec2[](\n"
		"    vec2(-1.0, -1.0),\n"
		"    vec2(-1.0, 3.0),\n"
		"    vec2(3.0, -1.0)\n"
		");\n"
		"\n"
		"vec2 tcs[3] = vec2[](\n"
		"    vec2(0.0, 0.0),\n"
		"    vec2(0.0, 2.0),\n"
		"    vec2(2.0, 0.0)\n"
		");\n"
		"\n"
		"void main() {\n"
		"    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);\n"
		"    outTexcoord = tcs[gl_VertexIndex];\n"
		"}";

	const char* fsCode =
		"#ParameterSet Present\n"
		"\n"
		"layout(location = 0) in vec2 inTexcoord;\n"
		"\n"
		"layout(location = 0) out vec4 outColor;\n"
		"\n"
		"void main() {\n"
		"    outColor = texture(Texture, inTexcoord);\n"
		"}";

	m_ParametrSet = renderCore.CreateParameterSet("Present");
	m_ParametrSet->DeclareTextureParameter("Texture");
	m_ParametrSet->Build();

	m_Pipeline = renderCore.CreateGraphicsPipeline();
	m_Pipeline->SetName("Present");
	m_Pipeline->DeclareVertexShader(vsCode);
	m_Pipeline->DeclareFragmentShader(fsCode);
	m_Pipeline->Build();

	return true;
}

void vk::PresentationPipeline::Release(vk::RenderCore& renderCore) {
	renderCore.DestroyGraphicsPipeline(m_Pipeline);
	renderCore.DestroyParameterSet(m_ParametrSet);
}

auto vk::PresentationPipeline::GetPipeline() const->vk::GraphicsPipeline* {
	return m_Pipeline;
}

auto vk::PresentationPipeline::GetParameterSet() const->vk::ParameterSet* {
	return m_ParametrSet;
}