#pragma once

namespace df {
	class FileData {
	public:
		explicit FileData(void* data, size_t size);
		FileData(const FileData&) = delete;
		FileData(FileData&& other);

		~FileData();

		auto GetData() const -> void* { return m_Data; }
		auto GetSize() const -> size_t { return m_Size; }

	private:
		void* m_Data;
		size_t m_Size;
	};
}