#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {

	template <typename T>
	class SparseStructureArray {
	public:
		SparseStructureArray() = default;
		SparseStructureArray(uint32 initialSize);
		~SparseStructureArray();

		auto operator[] (uint32 idx)->T&;
		auto operator[] (uint32 idx) const -> const T&;

		auto Add(T item)->uint32;
		void Remove(uint32 index);

	private:
		void Extend();

	private:
		df::Vector<T> m_Elements;
		df::Vector<uint32> m_Nexts;

		uint32 m_AvailableIndex = 0;
		uint32 m_Count = 0;
	};

	template <typename T>
	class SparseObjectArray {
	public:
		struct Element {
			Element() = default;
			Element(const Element&) = default;
			Element(Element&&) = default;
			Element& operator = (const Element&) = default;
			Element& operator = (Element&&) = default;

			T* m_Object = nullptr;
			uint32 m_Next = 0;
		};

	public:
		SparseObjectArray() = default;
		SparseObjectArray(uint32 initialSize);
		~SparseObjectArray();

		auto operator[] (uint32 idx)->T&;
		auto operator[] (uint32 idx) const -> const T&;

		template<typename ...TArgs>
		auto Add(TArgs&&... args)->uint32;
		void Remove(uint32 index);

	private:
		void Extend();

	private:
		df::Vector<Element> m_Elements;
		uint32 m_AvailableIndex = 0;
		uint32 m_Count = 0;
	};





	template <typename T, typename Enable = void>
	class SparseArray;

	template <typename T>
	class SparseArray<T, typename std::enable_if<std::is_trivial<T>::value>::type> : public SparseStructureArray<T> {
	public:
		SparseArray() = default;
		SparseArray(uint32 initialSize);
	};

	template <typename T>
	class SparseArray<T, typename std::enable_if<!std::is_trivial<T>::value>::type> : public SparseObjectArray<T> {
	public:
		SparseArray() = default;
		SparseArray(uint32 initialSize);
	};
}

#include <DwarvenCore/SparseArray.inl>