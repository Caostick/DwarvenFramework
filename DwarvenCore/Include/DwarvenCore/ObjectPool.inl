#pragma once

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

template<typename T, int SZ>
df::ObjectPool<T, SZ>::ObjectPool() 
	: m_Available(nullptr)
	, m_Count(0) {
	AddPage();
}

template<typename T, int SZ>
df::ObjectPool<T, SZ>::~ObjectPool() {
	DFAssert(m_Count == 0, "ObjectPool still contains objects!");

	for (auto page : m_Pages) {
		DFDelete page;
	}
}

template<typename T, int SZ>
template<typename... TArgs>
auto df::ObjectPool<T, SZ>::Create(TArgs&&... args)->T* {
	if (!m_Available->m_Next) {
		AddPage();
	}

	Container* container = m_Available;

	m_Available = m_Available->m_Next;

	T* object = reinterpret_cast<T*>(container->m_ObjectData);
	new (object) T(std::forward<TArgs>(args)...);
	m_Count++;

	return object;
}

template<typename T, int SZ>
void df::ObjectPool<T, SZ>::Destroy(T* object) {
	DFAssert(SearchPage(object), "Object doesn't belong to ObjectPool!");

	reinterpret_cast<T*>(object)->~T();
	m_Count--;

	Container* container = (Container*)object;

	if (container->m_Prev) {
		container->m_Prev->m_Next = container->m_Next;
	}

	if (container->m_Next) {
		container->m_Next->m_Prev = container->m_Prev;
	}

	Container* tail = m_Available->m_Prev;
	Container* available = m_Available;

	if (tail) {
		tail->m_Next = container;
	}
	available->m_Prev = container;

	container->m_Prev = tail;
	container->m_Next = available;

	m_Available = container;
}

template<typename T, int SZ>
auto df::ObjectPool<T, SZ>::Count() const->uint32 {
	return m_Count;
}

template<typename T, int SZ>
void df::ObjectPool<T, SZ>::Clear() {
	while (m_Available->m_Prev) {
		Destroy(reinterpret_cast<T*>(m_Available->m_Prev->m_ObjectData));
	}
}

template<typename T, int SZ>
void df::ObjectPool<T, SZ>::AddPage() {

	// Allocate page
	auto newPage = DFNew Page;
	m_Pages.push_back(newPage);

	// Connect page items
	for (int i = 0; i < SZ - 1; ++i) {
		newPage->m_Objects[i].m_Next = &(newPage->m_Objects[i + 1]);
	}

	for (int i = 1; i < SZ; ++i) {
		newPage->m_Objects[i].m_Prev = &(newPage->m_Objects[i - 1]);
	}

	newPage->m_Objects[0].m_Prev = m_Available;
	newPage->m_Objects[SZ - 1].m_Next = nullptr;

	// Connect tail
	if (m_Available) {
		m_Available->m_Next = &(newPage->m_Objects[0]);
	} else {
		m_Available = &(newPage->m_Objects[0]);
	}
}

template<typename T, int SZ>
bool df::ObjectPool<T, SZ>::SearchPage(T* object) const {
	for (auto page : m_Pages) {
		const uint8* objAddr = (uint8*)object;
		const uint8* minPageAddr = (uint8*)page;
		const uint8* maxPageAddr = (uint8*)page + sizeof(Page);

		if ((objAddr >= minPageAddr) && (objAddr < maxPageAddr)) {
			return true;
		}
	}

	return false;
}




template<typename T, int SZ>
df::ObjectPool<T, SZ>::Iterator::Iterator(Container* container)
	: m_Container(container) {}

template<typename T, int SZ>
auto df::ObjectPool<T, SZ>::Iterator::operator++() -> Iterator& {
	m_Container = m_Container->m_Prev;
	return *this;
}

template<typename T, int SZ>
bool df::ObjectPool<T, SZ>::Iterator::operator != (const Iterator&) const {
	return m_Container != nullptr;
}

template<typename T, int SZ>
auto df::ObjectPool<T, SZ>::Iterator::operator* () const -> T* {
	return reinterpret_cast<T*>(m_Container->m_ObjectData);
}

template<typename T, int SZ>
auto df::ObjectPool<T, SZ>::begin() const -> Iterator {
	return Iterator(m_Available->m_Prev);
}

template<typename T, int SZ>
auto df::ObjectPool<T, SZ>::end() const -> Iterator {
	return Iterator(nullptr);
}