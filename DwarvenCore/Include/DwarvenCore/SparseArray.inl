#pragma once

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

template <typename T>
df::SparseStructureArray<T>::SparseStructureArray(uint32 initialSize) {
	m_Elements.resize(initialSize);
	m_Nexts.resize(initialSize);
	for (uint32 i = 0; i < initialSize; ++i) {
		m_Nexts[i] = i + 1;
	}
}

template <typename T>
df::SparseStructureArray<T>::~SparseStructureArray() {
	DFAssert(m_Count == 0, "SparseArray still contains elements!");
}

template <typename T>
auto df::SparseStructureArray<T>::operator[](uint32 idx) -> T& {
	return m_Elements[idx];
}

template <typename T>
auto df::SparseStructureArray<T>::operator[](uint32 idx) const -> const T& {
	return m_Elements[idx];
}

template<typename T>
auto df::SparseStructureArray<T>::Add(T item) -> uint32 {
	if (m_AvailableIndex >= uint32(m_Elements.size())) {
		Extend();
	}

	const uint32 availableIndex = m_AvailableIndex;
	m_AvailableIndex = m_Nexts[availableIndex];
	m_Elements[availableIndex] = item;
	m_Count++;
	return availableIndex;
}

template <typename T>
void df::SparseStructureArray<T>::Remove(uint32 index) {
	DFAssert(m_Count > 0, "Trying to remove element from empty SparseArray!");

	m_Nexts[index] = m_AvailableIndex;
	m_AvailableIndex = index;

	m_Count--;
}

template <typename T>
void df::SparseStructureArray<T>::Extend() {
	const uint32 prevSize = uint32(m_Elements.size());
	const uint32 size = (prevSize < 16) ? 16 : (prevSize * 2);

	m_Elements.resize(size);
	m_Nexts.resize(size);
	for (uint32 i = prevSize; i < size; ++i) {
		m_Nexts[i] = i + 1;
	}
}







template <typename T>
df::SparseObjectArray<T>::SparseObjectArray(uint32 initialSize) {
	m_Elements.resize(initialSize);
	for (uint32 i = 0; i < initialSize; ++i) {
		m_Elements[i].m_Next = i + 1;
	}
}

template <typename T>
df::SparseObjectArray<T>::~SparseObjectArray() {
	DFAssert(m_Count == 0, "SparseArray still contains elements!");
}

template <typename T>
auto df::SparseObjectArray<T>::operator[](uint32 idx) -> T& {
	return *m_Elements[idx].m_Object;
}

template <typename T>
auto df::SparseObjectArray<T>::operator[](uint32 idx) const -> const T& {
	return *m_Elements[idx].m_Object;
}

template<typename T>
template<typename ...TArgs>
auto df::SparseObjectArray<T>::Add(TArgs&&... args) -> uint32 {
	if (m_AvailableIndex >= uint32(m_Elements.size())) {
		Extend();
	}

	const uint32 availableIndex = m_AvailableIndex;
	m_AvailableIndex = m_Elements[availableIndex].m_Next;
	m_Elements[availableIndex].m_Object = DFNew T(std::forward<TArgs>(args)...);
	m_Count++;
	return availableIndex;
}

template <typename T>
void df::SparseObjectArray<T>::Remove(uint32 index) {
	DFAssert(m_Count > 0, "Trying to remove element from empty SparseArray!");

	DFDelete m_Elements[index].m_Object;
	m_Elements[index].m_Object = nullptr;
	m_Elements[index].m_Next = m_AvailableIndex;
	m_AvailableIndex = index;

	m_Count--;
}

template <typename T>
void df::SparseObjectArray<T>::Extend() {
	const uint32 prevSize = uint32(m_Elements.size());
	const uint32 size = (prevSize < 16) ? 16 : (prevSize * 2);

	m_Elements.resize(size);
	for (uint32 i = prevSize; i < size; ++i) {
		m_Elements[i].m_Next = i + 1;
	}
}









template <typename T>
df::SparseArray<T, typename std::enable_if<std::is_trivial<T>::value>::type>::SparseArray(uint32 initialSize) 
	: SparseStructureArray<T>(initialSize) {}



template <typename T>
df::SparseArray<T, typename std::enable_if<!std::is_trivial<T>::value>::type>::SparseArray(uint32 initialSize) 
	: SparseObjectArray<T>(initialSize) {}