#pragma once

#include <DwarfFileSystem/File.h>

#include <stdio.h>

namespace df {
	class DefaultReadAccessFile : public df::File {
	public:
		explicit DefaultReadAccessFile(FILE* file);
		~DefaultReadAccessFile() override;

		auto MapData(size_t offset = 0, size_t size = 0) ->df::FileData override;
		void Write(void* buffer, size_t size) override;
		void Read(void* buffer, size_t size) override;

		auto GetSize() const -> size_t override;
	private:
		size_t m_Size;
		FILE* m_File;
	};
}