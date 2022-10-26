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
}

namespace df {
	class RenderPass {
	public:
		virtual ~RenderPass() {}

		virtual void SetName(const df::StringView& name) = 0;
		virtual void SetTarget(uint32 index, df::Texture* texture, df::ERenderTargetOp operation = df::ERenderTargetOp::Clear, const Vec4& clearValue = Vec4()) = 0;
		virtual void SetDepthStencilTarget(df::Texture* texture, df::ERenderTargetOp operation = df::ERenderTargetOp::Clear, float clearValue = 0.0f) = 0;

	private:
	};
}