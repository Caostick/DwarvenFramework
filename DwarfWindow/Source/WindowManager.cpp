#include <DwarfWindow/WindowManager.h>
#include <DwarfWindow/Window.h>

#include <DwarvenCore/Assert.h>

#ifdef GLFW_WINDOW_IMPLEMENTATION
#include <glfw/glfw3.h>
#endif


bool df::WindowManager::Init() {
	if (!glfwInit()) {
		DFAssert(false, "Can't initialize GLFW!");
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_Initialized = true;

	return true;
}

void df::WindowManager::Release() {
#ifdef GLFW_WINDOW_IMPLEMENTATION
	glfwTerminate();
#endif

	m_Initialized = false;
}

bool df::WindowManager::Update() {
	DFAssert(m_Initialized, "Window manager is not initialized!");

	glfwPollEvents();

	for (auto window : m_Windows) {
		window->Update();

		if (window->IsCloseRequested()) {
			m_ToClose.push_back(window);
		}
	}

	for (auto window : m_ToClose) {
		DestroyWindow(window);
	}
	m_ToClose.clear();

	return true;
}

void df::WindowManager::SwapBuffers() {
	DFAssert(m_Initialized, "Window manager is not initialized!");

	for (auto window : m_Windows) {
		window->SwapBuffers();
	}
}

auto df::WindowManager::GetWindowCount() const->uint32 {
	return m_Windows.Count();
}

auto df::WindowManager::CreateWindow(uint32 width, uint32 height)->df::Window* {
	DFAssert(m_Initialized, "Window manager is not initialized!");

	return m_Windows.Create(width, height);
}

void df::WindowManager::DestroyWindow(df::Window* window) {
	m_Windows.Destroy(window);
}