#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Signal.h>

#ifdef GLFW_WINDOW_IMPLEMENTATION
struct GLFWwindow;

namespace df {
	using APIWindow = GLFWwindow;
}
#endif


namespace df {
	class Window {
	public:
		Window();

		bool Init(uint32 width, uint32 height);
		void Release();

		bool Update();
		void SwapBuffers();

		auto GetPtr() const->APIWindow*;
		auto GetWidth() const->uint32;
		auto GetHeight() const->uint32;

		//void OnWindowSizeChanged(uint32 width, uint32 height);

	private:
		void InitSignals();

	private:

		APIWindow* m_Window;

		uint32 m_Width;
		uint32 m_Height;

	public:
		// width, height
		Signal<uint32, uint32> OnFramebufferSizeSignal;

		// key, scan code, state, mods
		Signal<int, int, int, int> OnKeySignal;

		// char code
		Signal<unsigned int> OnCharSignal;

		// button, state, mods
		Signal<int, int, int> OnMouseButtonSignal;

		// xOffset, yOffset
		Signal<double, double> OnScrollSignal;

		// mouseX, mouseY
		Signal<double, double> OnMouseMoveSignal;

		// count, files
		Signal<int, const char**> OnDropFilesSignal;
	};
}