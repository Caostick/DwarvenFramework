#pragma once

namespace df {
	template <typename T>
	void Log(const T& arg);

	template <typename T, typename ...ARGS>
	void Log(const T& arg, ARGS... args);

	namespace log {

		void Info(const char* message);
		void Warning(const char* message);
		void Error(const char* message);

		void InfoFmt(const char* fmt, ...);
		void WarningFmt(const char* fmt, ...);
		void ErrorFmt(const char* fmt, ...);

		void Print();
		void Clear();
		void Process();
	}
}

#include <DwarvenCore/Log.inl>