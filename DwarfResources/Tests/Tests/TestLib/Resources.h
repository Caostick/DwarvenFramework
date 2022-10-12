#pragma once

#include <DwarfResourcer/Resource.h>

#include <DwarvenCore/String.h>

class TestResourceAB : public df::IResource {
public:
	TestResourceAB() = default;
	TestResourceAB(const TestResourceAB&) = default;
	TestResourceAB(TestResourceAB&&) = default;
	TestResourceAB& operator = (const TestResourceAB&) = default;
	TestResourceAB& operator = (TestResourceAB&&) = default;

	void SetData(const char* data, uint32 dataSize);
	auto GetData() const -> const df::String&;

private:
	df::String m_Data;
};

class TestResourceC : public df::IResource {
public:
	TestResourceC() = default;
	TestResourceC(const TestResourceC&) = default;
	TestResourceC(TestResourceC&&) = default;
	TestResourceC& operator = (const TestResourceC&) = default;
	TestResourceC& operator = (TestResourceC&&) = default;

	void SetData(const char* data, uint32 dataSize);
	auto GetData() const -> const df::String&;

private:
	df::String m_Data;
};