#pragma once

#include <DwarfResources/FileData.h>

namespace df {
	class IFile {
	public:
		virtual ~IFile() {}

		virtual auto MapData(size_t offset = 0, size_t size = 0) -> df::FileData = 0;
		virtual void Write(void* buffer, size_t size) = 0;
		virtual void Read(void* buffer, size_t size) = 0;
		virtual auto GetSize() const -> size_t = 0;

		template<typename T>
		void Write(const T& value) {
			void* ptr = (void*)(&value);
			Write(ptr, sizeof(T));
		}

		template<typename T>
		T Read() {
			T value;
			Write(&value, sizeof(T));
			return value;
		}
	};
}