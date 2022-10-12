#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	class Module;
}

namespace df {
	class Application {
	public:
		template<typename T, typename... TArgs>
		bool EnableModule(TArgs&&... args);

		template<typename T>
		auto GetModule()->T*;

		auto Exec()->int32;
		void Close();

	private:
		df::Vector<Module*> m_Modules;
		bool m_CloseRequest = false;;
	};
}

#include <DwarvenCore/Application/Application.inl>