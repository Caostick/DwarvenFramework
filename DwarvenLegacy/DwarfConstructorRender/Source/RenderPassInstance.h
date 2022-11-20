#pragma once

#include <DwarfConstructorRender/ObjectId.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>

namespace rf {
	class RenderCore;
	class RenderPass;
	class ParamSet;
	class ModelStream;
}

namespace rf {
	class RenderPassInstance {
	public:
		RenderPassInstance(rf::RenderPass* renderPass, uint32 typeId);
		~RenderPassInstance();

		void Init(rf::RenderCore& renderCore, const df::Vector<rf::TextureId>& textures);
		void Release(rf::RenderCore& renderCore);
		void UpdateParams();

		void Bind(rf::ModelStream* modelStream);
		void Unbind(rf::ModelStream* modelStream);
	
		uint32 m_TypeId = 0;
		rf::RenderPass* m_RenderPass = nullptr;
		rf::ParamSet* m_ParamSet = nullptr;
		rf::FramebufferId m_Framebuffer = nullptr;

		df::Vector<rf::ModelStream*> m_BoundModelStreams;
	};
}