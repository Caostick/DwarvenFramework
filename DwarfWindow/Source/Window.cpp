#include <DwarfWindow/Window.h>

#include <DwarvenCore/Assert.h>

#ifdef GLFW_WINDOW_IMPLEMENTATION
#include <glfw/glfw3.h>
#endif

auto df::CreateWindow()->Window* {
	return new Window();
}

df::Window::Window()
	: m_Window(nullptr)
	, m_Width(0)
	, m_Height(0)
{}

bool df::Window::Init(uint32 width, uint32 height) {
	m_Width = width;
	m_Height = height;

#ifdef GLFW_WINDOW_IMPLEMENTATION
	if (!glfwInit()) {
		DFAssert(false, "Can't initialize GLFW!");
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	const char* windowName = "Game";
	const bool isFullscreen = false;
	m_Window = glfwCreateWindow(
		m_Width,
		m_Height,
		windowName,
		isFullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr
	);

	if (m_Window == nullptr) {
		DFAssert(false, "Can't create GLFW window!");
		return false;
	}

	glfwSetWindowUserPointer(m_Window, this);
	glfwMakeContextCurrent(m_Window);

	/*glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		df::Window* self = static_cast<df::Window*>(glfwGetWindowUserPointer(window));
		self->OnWindowSizeChanged(uint32(width), uint32(height));
		});*/
#endif

	InitSignals();

	return true;
}

void df::Window::Release() {
#ifdef GLFW_WINDOW_IMPLEMENTATION
	glfwDestroyWindow(m_Window);
	glfwTerminate();
#endif
}

bool df::Window::ShouldClose() const {
	return glfwWindowShouldClose(m_Window);
}

bool df::Window::Update() {
	DFAssert(m_Window != nullptr, "Window is not created!");

	glfwPollEvents();

	int w = 0;
	int h = 0;
	glfwGetWindowSize(m_Window, &w, &h);

	m_Width = uint32(w);
	m_Height = uint32(h);

	//double mouseX, mouseY;
	//glfwGetCursorPos(m_Window, &mouseX, &mouseY);

	return true;
}

void df::Window::SwapBuffers() {
	DFAssert(m_Window != nullptr, "Window is not created!");
	glfwSwapBuffers(m_Window);
}

auto df::Window::GetPtr() const->APIWindow* {
	return m_Window;
}

auto df::Window::GetWidth() const->uint32 {
	return m_Width;
}

auto df::Window::GetHeight() const->uint32 {
	return m_Height;
}

//void df::Window::OnWindowSizeChanged(uint32 width, uint32 height) {
//	m_Width = width;
//	m_Height = height;
//}

void df::Window::InitSignals() {
#ifdef GLFW_WINDOW_IMPLEMENTATION
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnFramebufferSizeSignal.Emit(uint32(width), uint32(height));
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int state, int mods) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnKeySignal.Emit(key, scancode, state, mods);
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int charCode) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnCharSignal.Emit(charCode);
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int state, int mods) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnMouseButtonSignal.Emit(button, state, mods);
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnScrollSignal.Emit(xOffset, yOffset);
		});

	glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char** files) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->OnDropFilesSignal.Emit(count, files);
		});
#endif
}