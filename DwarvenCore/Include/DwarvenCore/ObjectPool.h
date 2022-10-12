#pragma once

#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	template<typename T, int SZ = 32>
	class ObjectPool {
	public:
		static const int PageSize = SZ;

		ObjectPool();
		~ObjectPool();

		template<typename... TArgs>
		auto Create(TArgs&&... args)->T*;
		void Destroy(T* object);

		auto Count() const->uint32;
		void Clear();

	private:
		void AddPage();
		bool SearchPage(T* object) const;

		struct Container {
			uint8 m_ObjectData[sizeof(T)];

			Container* m_Next;
			Container* m_Prev;
		};

		struct Page {
			Container m_Objects[SZ];
		};

		df::Vector<Page*> m_Pages;
		Container* m_Available;
		uint32 m_Count;

		static_assert(std::is_class<T>::value, "Use ObjectPool for non trivial types!");

	public:
		class Iterator {
		public:
			Iterator() = delete;
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;
			Iterator(Container* container);

			auto operator++() -> Iterator&;
			bool operator != (const Iterator&) const;
			auto operator* () const->T*;

		private:
			Container* m_Container;
		};

		auto begin() const -> Iterator;
		auto end() const -> Iterator;
	};
}

#include <DwarvenCore/ObjectPool.inl>