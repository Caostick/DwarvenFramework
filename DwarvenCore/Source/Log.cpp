#include <DwarvenCore/Log.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/SpinLock.h>

#include <iostream>

#include <stdio.h>
#include <stdarg.h>

namespace {
	class Logger {
	public:
		auto GetEntries() const -> const df::Vector<df::String>& {
			return m_Entries;
		}

		void PushEntry(const char* entry) {
			m_Lock.Lock();
			m_Entries.emplace_back(df::String(entry));
			m_Lock.Unlock();
		}

		void Print() {
			m_Lock.Lock();
			for (auto&& entry : m_Entries) {
				std::cout << entry << "\n";
			}
			m_Lock.Unlock();
		}

		void Clear() {
			m_Lock.Lock();
			m_Entries.clear();
			m_Lock.Unlock();
		}
	private:
		df::Vector<df::String> m_Entries;
		df::SpinLock m_Lock;
	};

	Logger s_Logger;

	void LogMessage(const char* lType, const char* message) {
		char buffer[256];
		sprintf_s(buffer, "%s: %s", lType, message);

		s_Logger.PushEntry(buffer);
	}

	void LogMessageFmt(const char* lType, const char* fmt, va_list& args) {
		char buffer1[256];
		vsprintf_s(buffer1, fmt, args);

		char buffer2[256];
		sprintf_s(buffer2, "%s: %s", lType, buffer1);

		s_Logger.PushEntry(buffer2);
	}
}

void df::log::Info(const char* message) {
	LogMessage("Info", message);
}

void df::log::Warning(const char* message) {
	LogMessage("Warning", message);
}

void df::log::Error(const char* message) {
	LogMessage("Error", message);
}

void df::log::InfoFmt(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LogMessageFmt("Info", fmt, args);
}

void df::log::WarningFmt(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LogMessageFmt("Warning", fmt, args);
}

void df::log::ErrorFmt(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LogMessageFmt("Error", fmt, args);
}

void df::log::Print() {
	s_Logger.Print();
}

void df::log::Clear() {
	s_Logger.Clear();
}

void df::log::Process() {
	Print();
	Clear();
}