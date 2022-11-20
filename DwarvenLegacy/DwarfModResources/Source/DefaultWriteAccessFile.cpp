#include <DwarfResources/DefaultWriteAccessFile.h>

#include <DwarvenCore/New.h>
#include <DwarvenCore/Assert.h>

df::DefaultWriteAccessFile::DefaultWriteAccessFile(FILE* file)
	: m_File(file) {
}

df::DefaultWriteAccessFile::~DefaultWriteAccessFile() {
	fclose(m_File);
}

auto df::DefaultWriteAccessFile::MapData(size_t /*offset*/ /*= 0*/, size_t /*size*/ /*= 0*/) -> df::FileData {
	return df::FileData(nullptr, 0);
}

void df::DefaultWriteAccessFile::Write(void* buffer, size_t size) {
	fwrite(buffer, size, 1, m_File);
}

void df::DefaultWriteAccessFile::Read(void* /*buffer*/, size_t /*size*/) {
	DFAssert(false, "Trying to read data from write access file!");
}

auto df::DefaultWriteAccessFile::GetSize() const -> size_t {
	fseek(m_File, 0, SEEK_END);
	size_t size = ftell(m_File);
	fseek(m_File, 0, SEEK_SET);

	return size;
}

