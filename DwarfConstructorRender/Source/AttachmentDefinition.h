#pragma once

#include <DwarfConstructorRender/Types.h>

namespace rf {
	enum class EAttachmentLoadOp {
		DontCare,
		Clear,
		Load,

		COUNT
	};

	enum class EAttachmentStoreOp {
		DontCare,
		Store,

		COUNT
	};

	union ClearColorValue {
		float       Float32[4];
		int32_t     Int32[4];
		uint32_t    UInt32[4];
	};

	struct ClearDepthStencilValue {
		float       m_Depth;
		uint32_t    m_Stencil;
	};

	union ClearValue {
		rf::ClearColorValue ColorValue;
		rf::ClearDepthStencilValue DepthStencilValue;
	};

	struct AttachmentDefinition {
		AttachmentDefinition() = default;
		AttachmentDefinition(AttachmentDefinition&&) = default;
		AttachmentDefinition(const AttachmentDefinition&) = default;
		AttachmentDefinition& operator = (AttachmentDefinition&&) = default;
		AttachmentDefinition& operator = (const AttachmentDefinition&) = default;

		rf::ETextureFormat m_Format = rf::ETextureFormat::Undefined;

		rf::EImageLayout m_InitialLayout = rf::EImageLayout::Undefined;
		rf::EImageLayout m_FinalLayout = rf::EImageLayout::Undefined;

		rf::EAttachmentLoadOp m_LoadOp = rf::EAttachmentLoadOp::DontCare;
		rf::EAttachmentStoreOp m_StoreOp = rf::EAttachmentStoreOp::DontCare;
		rf::EAttachmentLoadOp m_StencilLoadOp = rf::EAttachmentLoadOp::DontCare;
		rf::EAttachmentStoreOp m_StencilStoreOp = rf::EAttachmentStoreOp::DontCare;

		rf::ClearValue m_ClearValue;
	};
}