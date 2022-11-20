#include <DwarfResources/DefaultFileSystem.h>
#include <DwarfResources/DefaultReadAccessFile.h>
#include <DwarfResources/DefaultWriteAccessFile.h>

#include <DwarvenCore/FileNameOperations.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/New.h>

#include <filesystem>

auto df::DefaultFileSystem::GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> {
	const df::String actualPath = df::String(directory);

	df::Vector<df::String> files;

	if (!std::filesystem::exists(actualPath.c_str())) {
		return files;
	}

	for (auto& file : std::filesystem::recursive_directory_iterator(actualPath.c_str())) {
		if (file.is_directory()) {
			continue;
		}

		files.push_back(df::OptimizePath<char>(file.path().string()));
	}

	return files;
}

auto df::DefaultFileSystem::OpenFile(const df::StringView& filePath, EFileAccess access) const -> df::IFile* {
	const df::String actualPath = df::String(filePath);

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

	return nullptr;
}

void df::DefaultFileSystem::CloseFile(df::IFile* file) const {
	DFDelete file;
}

bool df::DefaultFileSystem::CreateDirectory(const df::StringView& dirPath) const {
	const df::String actualPath = df::String(dirPath);
	return std::filesystem::create_directory(actualPath.c_str());
}