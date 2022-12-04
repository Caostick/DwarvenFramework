#include <DwarfFileSystem/VirtualFileSystem.h>
#include <DwarfFileSystem/File.h>

#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/FileNameOperations.h>

df::VirtualFileSystem::VirtualDirectory::VirtualDirectory(FileSystem& fileSystem)
	: m_FileSystem(fileSystem) {
}



auto df::VirtualFileSystem::GetFilesRecursive(const StringView& directory) const->Vector<String> {
	auto&& [actualDir, fileSystem] = ToActualPath(directory);
	if (!fileSystem) {
		return {};
	}

	auto files = fileSystem->GetFilesRecursive(actualDir);
	for (auto& file : files) {
		file = ToVirtualPath(FixSlash<char>(file));
	}

	return files;
}

auto df::VirtualFileSystem::OpenFile(const StringView& filePath, EFileAccess access) const->File* {
	auto&& [actualPath, fileSystem] = ToActualPath(filePath);
	if (!fileSystem) {
		return nullptr;
	}

	return fileSystem->OpenFile(actualPath, access);
}

void df::VirtualFileSystem::CloseFile(File* file) const {
	auto fileSystem = file->GetFileSystem();
	if (fileSystem) {
		fileSystem->CloseFile(file);
	}
}

bool df::VirtualFileSystem::CreateDirectory(const StringView& dirPath) const {
	auto&& [actualPath, fileSystem] = ToActualPath(dirPath);
	if (!fileSystem) {
		return false;
	}
	return fileSystem->CreateDirectory(actualPath);
}

auto df::VirtualFileSystem::GetUserLocalDirectory() const->String {
	return String("");
}

void df::VirtualFileSystem::CreateVirtualDirectory(const StringView& virtualDir, const StringView& actualDir, FileSystem& fileSystem) {
	VirtualDirectory dir(fileSystem);
	dir.m_VirtualDirectory = String(FixSlash<char>(Trim(virtualDir, "/\\")));
	dir.m_ActualDirectory = String(FixSlash<char>(Trim(actualDir, "/\\")));

	for (auto&& vDir : m_VirtualDirectories) {
		if (vDir.m_VirtualDirectory == dir.m_ActualDirectory) {
			return;
		}
	}

	// @TODO: Sort directories to make possible v dirs like "Data/Shaders/" along "Data/"

	m_VirtualDirectories.emplace_back(dir);
}

auto df::VirtualFileSystem::ToActualPath(const StringView& virtualPath) const -> std::tuple<String, FileSystem*> {
	for (auto&& vDir : m_VirtualDirectories) {
		const auto& vPrefix = vDir.m_VirtualDirectory;
		const auto len = vPrefix.length();
		if (TakeLeft<char>(virtualPath, len) == vPrefix) {
			const auto& rightPart = DropLeft<char>(virtualPath, len);
			return std::make_tuple(vDir.m_ActualDirectory + String(rightPart), &vDir.m_FileSystem);
		}
	}

	return std::make_tuple(String(""), nullptr);
}

auto df::VirtualFileSystem::ToVirtualPath(const StringView& actualPath) const ->String {
	for (auto&& vDir : m_VirtualDirectories) {
		const auto& aPrefix = vDir.m_ActualDirectory;
		const auto len = aPrefix.length();
		if (TakeLeft<char>(actualPath, len) == aPrefix) {
			const auto& rightPart = DropLeft<char>(actualPath, len);
			return vDir.m_VirtualDirectory + String(rightPart);
		}
	}

	return String("");
}