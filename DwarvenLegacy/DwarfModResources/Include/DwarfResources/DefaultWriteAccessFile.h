#pragma once

#include <DwarfResources/IFile.h>

#include <stdio.h>

namespace df {
	class DefaultWriteAccessFile : public df::IFile {
	public:
		explicit DefaultWriteAccessFile(FILE* file);
		~DefaultWriteAccessFile() override;

		auto MapData(size_t offset = 0, size_t size = 0) ->df::FileData override;
		void Write(void* buffer, size_t size) override;
		void Read(void* buffer, size_t size) override;

		auto GetSize() const ->size_t override;

	private:
		FILE* m_File;
	};
}