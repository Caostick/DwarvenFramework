#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
	class Texture;
}

namespace df {
	enum class ERenderTargetOp {
		DontCare,
		Clear,
		Load
	};

	struct ClearColor {
		float Red = 0.0f;
		float Green = 0.0f;
		float Blue = 0.0f;
		float Alpha = 0.0f;
	};

	struct ClearDepth {
		float Depth = 0.0f;
		uint32 Stencil = 0;
	};
}

namespace df {
	class RenderPass {
	public:
		virtual ~RenderPass() {}

		virtual void SetName(const df::StringView& name) = 0;
		virtual void SetTarget(uint32 index, df::Texture* texture, df::ERenderTargetOp operation = df::ERenderTargetOp::Clear, const ClearColor& clearValue = ClearColor()) = 0;
		virtual void SetDepthStencilTarget(df::Texture* texture, df::ERenderTargetOp operation = df::ERenderTargetOp::Clear, const ClearDepth& clearValue = ClearDepth()) = 0;

	private:
	};
}