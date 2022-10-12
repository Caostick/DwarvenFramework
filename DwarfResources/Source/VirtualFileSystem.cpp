#include <DwarfResources/VirtualFileSystem.h>
#include <DwarfResources/DefaultReadAccessFile.h>
#include <DwarfResources/DefaultWriteAccessFile.h>

#include <DwarvenCore/FileNameOperations.h>
#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

#include <filesystem>
#include <iostream>

void df::VirtualFileSystem::Init(const df::StringView& applicationPath) {
	m_Folders = df::SparseArray<VirtualFolder>(1024);

	m_ApplicationDir = applicationPath;

	m_RootFolderIndex = m_Folders.Add();
	m_Folders[m_RootFolderIndex].m_Name = ".";
}

void df::VirtualFileSystem::Release() {

}

void df::VirtualFileSystem::MapDirectory(const df::String& virtDir, const df::String& dir) {
	const int folderIndex = CreateDirectory(virtDir);
	m_Folders[folderIndex].m_Directory = df::OptimizePath(df::Trim(df::StringView(dir), "/"));
}

auto df::VirtualFileSystem::GetDirectory(const df::StringView& virtDir) const -> const df::String& {
	int curFolder = m_RootFolderIndex;
	for (const auto fld : df::Tokenize(df::DropLeft(virtDir, 2), "/")) {
		const int subFldIdx = FindFolder(curFolder, fld);
		if (subFldIdx != -1) {
			curFolder = subFldIdx;
		} else {
			break;
		}
	}

	if (curFolder != -1) {
		const auto& folder = m_Folders[curFolder];
		return folder.m_Directory;
	}

	return m_ApplicationDir;
}

auto df::VirtualFileSystem::GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> {
	const auto dir = GetDirectory(directory);

	df::Vector<df::String> files;
	for (auto& file : std::filesystem::recursive_directory_iterator(dir.data())) {
		if (file.is_directory()) {
			continue;
		}

		files.push_back(file.path().string());
	}

	// @TODO: convert each filepath to virtual filepath

	return files;
}

auto df::VirtualFileSystem::OpenFile(const df::StringView& filePath, EFileAccess access) const -> df::IFile* {
	size_t offset = 2;

	int curFolder = m_RootFolderIndex;
	for (const auto fld : df::Tokenize(df::DropLeft(filePath, 2), "/")) {
		const int subFldIdx = FindFolder(curFolder, fld);
		if (subFldIdx != -1) {
			curFolder = subFldIdx;
			offset += fld.length() + 1;
		} else {
			break;
		}
	}

	if (curFolder != -1) {
		const auto& folder = m_Folders[curFolder];
		if (!folder.m_Directory.empty()) {
			const df::String actualPath = folder.m_Directory + df::String("/") + df::String(filePath.data() + offset, filePath.length() - offset);

			FILE* fileHandle = nullptr;

			switch (access) {
			case df::EFileAccess::Read:
				fopen_s(&fileHandle, actualPath.c_str(), "rb");
				if (fileHandle) {
					return DFNew df::DefaultReadAccessFile(fileHandle);
				}
				break;
			case df::EFileAccess::Write:
				fopen_s(&fileHandle, actualPath.c_str(), "wb");
				if (fileHandle) {
					return DFNew df::DefaultWriteAccessFile(fileHandle);
				}
				break;
			default:
				break;
			}
		}
	}

	return nullptr;
}

void df::VirtualFileSystem::CloseFile(df::IFile* file) const {
	DFDelete file;
}

void df::VirtualFileSystem::Print(int folderIndex /*= -1*/, int depth /*= 0*/) const {
	if (folderIndex == -1) {
		folderIndex = m_RootFolderIndex;
	}

	const df::VirtualFolder& folder = m_Folders[folderIndex];

	for (int i = 0; i < depth; ++i) {
		std::cout << " ";
	}
	std::cout << folder.m_Name;
	std::cout << "/";

	std::cout << "    ";
	std::cout << "[" << folder.m_Directory << "]";

	std::cout << "\n";

	if (folder.m_FirstChildIndex != -1) {
		Print(folder.m_FirstChildIndex, depth + 1);
	}

	if (folder.m_SiblingIndex != -1) {
		Print(folder.m_SiblingIndex, depth);
	}
}

auto df::VirtualFileSystem::CreateDirectory(const df::StringView& directoryPath) -> int {
	const char* trim = "./\\";
	const char* sep = "/\\";

	const df::StringView directory = df::Trim(directoryPath, trim);

	int curFolderIndex = m_RootFolderIndex;

	for (const auto folder : df::Tokenize(directory, sep)) {
		int folderIndex = FindFolder(curFolderIndex, folder);
		if (folderIndex == -1) {
			folderIndex = CreateFolder(curFolderIndex, folder);
		}

		curFolderIndex = folderIndex;
	}

	return curFolderIndex;
}

auto df::VirtualFileSystem::FindFolder(int curFolderIdx, const df::StringView& folderName) const -> int {
	DFAssert(curFolderIdx >= 0, "Invalid folder index!");

	int childFolderIdx = m_Folders[curFolderIdx].m_FirstChildIndex;

	while (childFolderIdx != -1) {
		if (folderName == m_Folders[childFolderIdx].m_Name) {
			return childFolderIdx;
		}

		childFolderIdx = m_Folders[childFolderIdx].m_SiblingIndex;
	}

	return childFolderIdx;
}

auto df::VirtualFileSystem::CreateFolder(int curFolderIdx, const df::StringView& folderName) -> int {
	const int newFolder = m_Folders.Add();
	m_Folders[newFolder].m_Name = df::String(folderName.data(), folderName.length());
	m_Folders[newFolder].m_SiblingIndex = m_Folders[curFolderIdx].m_FirstChildIndex;

	m_Folders[curFolderIdx].m_FirstChildIndex = newFolder;

	return newFolder;
}