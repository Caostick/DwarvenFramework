#pragma once

#include <atomic>

namespace df {
	class SpinLock {
	public:
		SpinLock() = default;

		void Lock();
		bool TryLock();
		void Unlock();

	private:
		std::atomic_flag m_Lock = ATOMIC_FLAG_INIT;
	};
}