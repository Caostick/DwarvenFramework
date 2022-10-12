#include "RenderPassInstance.h"
#include "RenderCore.h"

#include <DwarfRender/ParamSet.h>
#include <DwarfRender/RenderPass.h>

#include <DwarvenCore/New.h>

rf::RenderPassInstance::RenderPassInstance(rf::RenderPass* renderPass, uint32 typeId) 
	: m_TypeId(typeId)
	, m_RenderPass(renderPass) {
}

rf::RenderPassInstance::~RenderPassInstance() {
	DFDelete m_RenderPass;
}

void rf::RenderPassInstance::Init(rf::RenderCore& renderCore, const df::Vector<rf::TextureId>& textures) {
	const auto typeId = m_TypeId;
	const auto entry = rf::RenderPassRegistry::Entries[typeId];
	const auto paramSetDef = entry.m_ParamSetDefinition;
	if (paramSetDef) {
		m_ParamSet = DFNew rf::ParamSet(*entry.m_ParamSetDefinition);
		m_ParamSet->Init(renderCore, false);

		m_Framebuffer = renderCore.CreateFramebuffer(entry.m_Name, entry.m_RenderPass, textures);
	}
}

void rf::RenderPassInstance::Release(rf::RenderCore& renderCore) {
	if (m_ParamSet) {
		m_ParamSet->Release(renderCore);
		DFDelete m_ParamSet;
		m_ParamSet = nullptr;
	}
}

void rf::RenderPassInstance::UpdateParams() {
	if (!m_ParamSet) {
		return;
	}

	m_RenderPass->UpdateParams(*m_ParamSet);
}

void rf::RenderPassInstance::Bind(rf::ModelStream* modelStream) {
	df::AddUnique(m_BoundModelStreams, modelStream);
}

void rf::RenderPassInstance::Unbind(rf::ModelStream* modelStream) {
	df::RemoveOrdered(m_BoundModelStreams, modelStream);
}