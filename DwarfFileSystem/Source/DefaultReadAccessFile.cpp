#include <DwarfFileSystem/DefaultReadAccessFile.h>

#include <DwarvenCore/New.h>

df::DefaultReadAccessFile::DefaultReadAccessFile(FILE* file)
	: m_Size(0)
	, m_File(file) {
	fseek(m_File, 0, SEEK_END);
	m_Size = ftell(m_File);
	fseek(m_File, 0, SEEK_SET);
}

df::DefaultReadAccessFile::~DefaultReadAccessFile() { fclose(m_File); }

auto df::DefaultReadAccessFile::MapData(size_t offset /*= 0*/,
	size_t size /*= 0*/) -> df::FileData {
	if (size == 0) {
		size = m_Size - offset;
	}

	fseek(m_File, (long)offset, SEEK_SET);
	void* data = DFNew char[size];
	Read(data, size);
	fseek(m_File, 0, SEEK_SET);

	return df::FileData(data, size);
}

void df::DefaultReadAccessFile::Write(void* buffer, size_t size) {
	fwrite(buffer, size, 1, m_File);
}

void df::DefaultReadAccessFile::Read(void* buffer, size_t size) {
	fread(buffer, size, 1, m_File);
}

auto df::DefaultReadAccessFile::GetSize() const -> size_t { return m_Size; }
