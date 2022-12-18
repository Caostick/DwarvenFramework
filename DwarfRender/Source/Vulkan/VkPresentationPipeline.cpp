#include "VkPresentationPipeline.h"
#include "VkRenderCore.h"
#include "VkParameterSet.h"
#include "VkGraphicsPipeline.h"
#include "Generated/Presentation.generated.h"

bool vk::PresentationPipeline::Init(vk::RenderCore& renderCore) {
	m_ParametrSet = renderCore.CreateParameterSet("Present");
	m_ParametrSet->DeclareTextureParameter("Texture");
	m_ParametrSet->Build();

	df::Vector<uint32> vsSpirV(generated::presentation::vsCodeLength);
	memcpy(vsSpirV.data(), generated::presentation::vsCodeData, sizeof(uint32) * generated::presentation::vsCodeLength);

	df::Vector<uint32> fsSpirV(generated::presentation::fsCodeLength);
	memcpy(fsSpirV.data(), generated::presentation::fsCodeData, sizeof(uint32) * generated::presentation::fsCodeLength);

	m_Pipeline = renderCore.CreateGraphicsPipeline();
	m_Pipeline->SetName("Present");
	m_Pipeline->SetupParameterSets({"Present"});
	m_Pipeline->SetupVertexShader(vsSpirV);
	m_Pipeline->SetupFragmentShader(fsSpirV);
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