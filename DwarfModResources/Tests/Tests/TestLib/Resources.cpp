#include "Resources.h"

void TestResourceAB::SetData(const char* data, uint32 dataSize) {
	m_Data = df::String(data, dataSize);
}

auto TestResourceAB::GetData() const -> const df::String& {
	return m_Data;
}



void TestResourceC::SetData(const char* data, uint32 dataSize) {
	m_Data = df::String(data, dataSize);
}

auto TestResourceC::GetData() const -> const df::String& {
	return m_Data;
}