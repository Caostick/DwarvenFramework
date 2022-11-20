#include <Core/Structures/SpinLock.h>

void df::SpinLock::Lock() {
	while (!TryLock()) {}
}

bool df::SpinLock::TryLock() {
	return !m_Lock.test_and_set(std::memory_order_acquire);
}

void df::SpinLock::Unlock() {
	m_Lock.clear(std::memory_order_release);
}