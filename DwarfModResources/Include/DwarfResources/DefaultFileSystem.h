#pragma once

#include <DwarfResources/IFileSystem.h>

namespace df {
	class DefaultFileSystem : public df::IFileSystem {
	public:
		auto GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> override;
		auto OpenFile(const df::StringView& filePath, EFileAccess access) const -> df::IFile* override;
		void CloseFile(df::IFile* file) const override;
		bool CreateDirectory(const df::StringView& dirPath) const override;
	};
}