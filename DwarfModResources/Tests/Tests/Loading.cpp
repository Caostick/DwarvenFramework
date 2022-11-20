#include <catch2/catch.hpp>

#include <DwarfResourcer/DefaultFileSystem.h>
#include <DwarfResourcer/ResourceManager.h>
#include <DwarfResourcer/Data.h>
#include <DwarfResourcer/ResourceLoader.h>

#include <DwarvenCore/String.h>

#include "TestLib/Resources.h"
#include "TestLib/Loaders.h"

TEST_CASE("FileSystem") {
	const df::String dataPath = "../../Tests/Data/";

	df::DefaultFileSystem fileSystem;

	const auto files = fileSystem.GetFilesRecursive(dataPath);
	REQUIRE(files.size() == 7);
}

TEST_CASE("Data Parsing") {
	const df::String dataPath = "../../Tests/Data/";

	df::DefaultFileSystem fileSystem;

	const auto files = fileSystem.GetFilesRecursive(dataPath);

	df::Data data(fileSystem);
	data.Parse(dataPath);

	REQUIRE(data.GetFileCount() == 2);
	REQUIRE(data.GetModuleCount() == 4);
	REQUIRE(data.GetExtensionCount() == 3);

	// Resources
	bool hasResource01 = false;
	bool hasResource02 = false;
	for (uint32 i = 0; i < data.GetFileCount(); ++i) {
		hasResource01 |= (data.GetFile(i).m_Filename == "resource01");
		hasResource02 |= (data.GetFile(i).m_Filename == "resource02");
	}
	REQUIRE(hasResource01);
	REQUIRE(hasResource02);

	for (uint32 i = 0; i < data.GetFileCount(); ++i) {
		if (data.GetFile(i).m_Filename == "resource01") {
			REQUIRE(data.GetFile(i).m_Instances.size() == 5);
		} else if (data.GetFile(i).m_Filename == "resource02") {
			REQUIRE(data.GetFile(i).m_Instances.size() == 2);
		}
	}

	// Modules
	bool hasModuleA = false;
	for (uint32 i = 0; i < data.GetModuleCount(); ++i) {
		hasModuleA |= (data.GetModule(i) == "ModuleA");
	}
	REQUIRE(hasModuleA);

	bool hasModuleB = false;
	for (uint32 i = 0; i < data.GetModuleCount(); ++i) {
		hasModuleB |= (data.GetModule(i) == "ModuleB");
	}
	REQUIRE(hasModuleB);

	bool hasModuleC = false;
	for (uint32 i = 0; i < data.GetModuleCount(); ++i) {
		hasModuleC |= (data.GetModule(i) == "ModuleC");
	}
	REQUIRE(hasModuleC);

	bool hasModuleD = false;
	for (uint32 i = 0; i < data.GetModuleCount(); ++i) {
		hasModuleD |= (data.GetModule(i) == "ModuleD");
	}
	REQUIRE(hasModuleD);

	// Extensions
	bool hasExtA = false;
	for (uint32 i = 0; i < data.GetExtensionCount(); ++i) {
		hasExtA |= (data.GetExtension(i) == "fmtA");
	}
	REQUIRE(hasExtA);

	bool hasExtB = false;
	for (uint32 i = 0; i < data.GetExtensionCount(); ++i) {
		hasExtB |= (data.GetExtension(i) == "fmtB");
	}
	REQUIRE(hasExtB);
}

TEST_CASE("ResourceManager Module Inheritance") {
	const df::String dataPath = "../../Tests/Data/";

	df::DefaultFileSystem fileSystem;
	df::ResourceManager resourceManager(fileSystem, dataPath);

	resourceManager.RegisterResourceLoader<TestResourceAB, FmtALoader>();
	resourceManager.RegisterResourceLoader<TestResourceAB, FmtBLoader>();
	resourceManager.RegisterResourceLoader<TestResourceC, FmtCLoader>();

	resourceManager.SetupModules({ "ModuleA", "ModuleC", "ModuleD" });
	resourceManager.ParseData();
	resourceManager.MapResources();

	const auto res01 = resourceManager.GetResource<TestResourceAB>("resource01");
	REQUIRE(res01);
	REQUIRE(res01->GetData() == "ModuleC:resource01.fmtA");

	const auto res02 = resourceManager.GetResource<TestResourceAB>("resource02");
	REQUIRE(res02);
	REQUIRE(res02->GetData() == "ModuleD:resource02.fmtA");

	const auto res03 = resourceManager.GetResource<TestResourceC>("resource03");
	REQUIRE(!res03);
}

TEST_CASE("ResourceManager ExtensionInheritance") {
	const df::String dataPath = "../../Tests/Data/";

	df::DefaultFileSystem fileSystem;
	df::ResourceManager resourceManager(fileSystem, dataPath);

	resourceManager.RegisterResourceLoader<TestResourceAB, FmtALoader>();
	resourceManager.RegisterResourceLoader<TestResourceAB, FmtBLoader>();
	resourceManager.RegisterResourceLoader<TestResourceC, FmtCLoader>();

	resourceManager.SetupModules({ "ModuleB" });
	resourceManager.ParseData();
	resourceManager.MapResources();

	const auto res01AB = resourceManager.GetResource<TestResourceAB>("resource01");
	REQUIRE(res01AB);
	REQUIRE(res01AB->GetData() == "ModuleB:resource01.fmtB");

	const auto res01C = resourceManager.GetResource<TestResourceC>("resource01");
	REQUIRE(res01C);
	REQUIRE(res01C->GetData() == "ModuleB:resource01.fmtC");
}