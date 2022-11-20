#pragma once

#include <DwarvenCore/SpinLock.h>

namespace df {
	template<typename T, size_t capacity>
	class ThreadSafeRingBuffer {
	public:
		ThreadSafeRingBuffer() = default;

		bool PushBack(const T& item);
		bool PopFront(T& item);

	private:
		T m_Data[capacity];
		size_t m_Head = 0;
		size_t m_Tail = 0;
		df::SpinLock m_Lock;
	};
}

#include <DwarvenCore/ThreadSafeRingBuffer.inl>