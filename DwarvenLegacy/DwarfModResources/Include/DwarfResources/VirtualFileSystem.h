#pragma once

#include <DwarfResources/IFileSystem.h>

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/SparseArray.h>
#include <DwarvenCore/Vector.h>

namespace df {
	class IFile;
}

namespace df {
	struct VirtualFolder {
		VirtualFolder() = default;
		VirtualFolder(const VirtualFolder&) = default;
		VirtualFolder(VirtualFolder&&) = default;

		VirtualFolder& operator = (const VirtualFolder&) = default;
		VirtualFolder& operator = (VirtualFolder&&) = default;

		df::String m_Name;
		df::String m_Directory;
		int m_DirectoryIndex = -1;
		int m_FirstChildIndex = -1;
		int m_SiblingIndex = -1;
	};

	class VirtualFileSystem : public df::IFileSystem {
	public:
		void Init(const df::StringView& applicationPath);
		void Release();

		void MapDirectory(const df::String& virtDir, const df::String& dir);
		auto GetDirectory(const df::StringView& virtDir) const -> const df::String&;

		auto GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> override;
		auto OpenFile(const df::StringView& filePath, EFileAccess access) const -> df::IFile* override;
		void CloseFile(df::IFile* file) const override;

		void Print(int folderIndex = -1, int depth = 0) const;

	private:
		auto CreateDirectory(const df::StringView& directoryPath) -> int;
		auto FindFolder(int curFolderIdx, const df::StringView& folderName) const -> int;
		auto CreateFolder(int curFolderIdx, const df::StringView& folderName) -> int;

		df::String m_ApplicationDir;
		df::SparseArray<VirtualFolder> m_Folders;
		int m_RootFolderIndex;
	};
}