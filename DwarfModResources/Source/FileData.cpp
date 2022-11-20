#include <DwarfResources/FileData.h>

#include <DwarvenCore/New.h>

#include <type_traits>

df::FileData::FileData(void* data, size_t size) 
	: m_Data(data)
	, m_Size(size) {
}

df::FileData::FileData(FileData&& other)
	: m_Data(std::move(other.m_Data))
	, m_Size(std::move(other.m_Size)) {
	other.m_Data = nullptr;
}

df::FileData::~FileData() {
	DFDelete [] m_Data;
}