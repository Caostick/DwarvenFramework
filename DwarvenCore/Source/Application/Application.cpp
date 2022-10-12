#include <DwarvenCore/Application/Application.h>
#include <DwarvenCore/Application/Module.h>

#include <DwarvenCore/Assert.h>

auto df::Application::Exec()->int32 {
	for (auto module : m_Modules) {
		if (!module->InitModule(*this)) {
			DFAssert(false, "Can't initialize module!");

			return -1;
		}
	}

	for (auto module : m_Modules) {
		if (!module->LoadModule(*this)) {
			DFAssert(false, "Can't initialize module!");

			return -1;
		}
	}

	while (!m_CloseRequest) {
		for (auto module : m_Modules) {
			module->Update(*this);
		}
	}

	for (auto module : m_Modules) {
		module->UnloadModule(*this);
	}

	for (auto module : m_Modules) {
		module->ReleaseModule(*this);
	}

	for (auto module : m_Modules) {
		DFDelete module;
	}
	m_Modules.clear();

	return 0;
}

void df::Application::Close() {
	m_CloseRequest = true;
}