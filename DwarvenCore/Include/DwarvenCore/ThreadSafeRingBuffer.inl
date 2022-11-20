#pragma once

template<typename T, size_t capacity>
bool df::ThreadSafeRingBuffer<T, capacity>::PushBack(const T& item) {
	bool result = false;
	m_Lock.Lock();
	size_t next = (m_Head + 1) % capacity;
	if (next != m_Tail) {
		m_Data[m_Head] = item;
		m_Head = next;
		result = true;
	}
	m_Lock.Unlock();
	return result;
}

template<typename T, size_t capacity>
bool df::ThreadSafeRingBuffer<T, capacity>::PopFront(T& item) {
	bool result = false;
	m_Lock.Lock();
	if (m_Tail != m_Head) {
		item = m_Data[m_Tail];
		m_Tail = (m_Tail + 1) % capacity;
		result = true;
	}
	m_Lock.Unlock();
	return result;
}