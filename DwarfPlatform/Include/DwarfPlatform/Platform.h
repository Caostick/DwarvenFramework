#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/ObjectPool.h>
#include <DwarvenCore/Vector.h>

namespace df {
	class Window;
}

namespace df {
	class Platform {
	public:
		bool Init();
		void Release();

		bool Update();
		void SwapBuffers();

		auto GetWindowCount() const->uint32;

		auto CreateWindow(uint32 width, uint32 height)->df::Window*;
		void DestroyWindow(df::Window* window);

		auto GetTime() const -> double;

	private:
		df::ObjectPool<df::Window> m_Windows;
		df::Vector<df::Window*> m_ToClose;

		bool m_Initialized = false;
	};
}