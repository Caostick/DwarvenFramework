#pragma once

#include <DwarvenCore/StringView.h>

namespace df {
	class RenderPass {
	public:
		virtual ~RenderPass() {}

		virtual void SetName(const df::StringView& name) = 0;

	private:
	};
}