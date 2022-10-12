#pragma once

#include <DwarvenCore/New.h>
#include <DwarvenCore/Types.h>
#include <DwarvenCore/Assert.h>

namespace {
	template<typename T>
	struct ModuleInfo {
		static int32 Id;
	};

	template<typename T>
	int32 ModuleInfo<T>::Id = -1;
}

template<typename T, typename... TArgs>
bool df::Application::EnableModule(TArgs&&... args) {
	static_assert(std::is_base_of<df::Module, T>::value);
	DFAssert(ModuleInfo<T>::Id == -1, "Module already enabled!");

	ModuleInfo<T>::Id = int32(m_Modules.size());
	m_Modules.push_back(DFNew T(std::forward<TArgs>(args)...));

	return true;
}

template<typename T>
auto df::Application::GetModule()->T* {
	DFAssert(ModuleInfo<T>::Id != -1, "Module wasn't enabled!");

	return static_cast<T*>(m_Modules[ModuleInfo<T>::Id]);
}