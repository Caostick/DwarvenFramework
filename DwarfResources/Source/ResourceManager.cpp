#include <DwarfResources/ResourceManager.h>
#include <DwarfResources/Data.h>

#include <DwarfFileSystem/FileSystem.h>
#include <DwarfFileSystem/File.h>

#include <DwarvenCore/FileNameOperations.h>
#include <DwarvenCore/StringOperations.h>

#include <assert.h>

df::ResourceManager::ResourceManager(const FileSystem& fileSystem, const StringView& dataPath) 
	: m_FileSystem(fileSystem)
	, m_DataPath(FixSlash<char>(dataPath)) {
}

df::ResourceManager::~ResourceManager() {
	for (auto tManager : m_ResourceTypeManagers) {
		DFDelete tManager;
	}
}

bool df::ResourceManager::IndexData() {
	const auto files = m_FileSystem.GetFilesRecursive(m_DataPath);
	for (auto& file : files) {
		const auto path = file;
		const auto guid = ToLower<char>(GetRelative<char>(m_DataPath, path));
		const auto ext = ToLower<char>(GetExtension<char>(path));

		for (auto&& tManager : m_ResourceTypeManagers) {
			tManager->PushResource(guid, path, ext);
		}
	}
	return true;
}

void df::ResourceManager::Clear() {
	for (auto&& tManager : m_ResourceTypeManagers) {
		tManager->Clear();
	}

}

void df::ResourceManager::Update() {
	for (auto&& tManaget : m_ResourceTypeManagers) {
		tManaget->Update();
	}
}