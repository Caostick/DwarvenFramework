#pragma once

namespace df {
	class Renderer;
}

namespace df {
	class Window;
}

namespace df {
	auto CreateRenderer(df::Window& window)->df::Renderer*;
}