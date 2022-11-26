#pragma once

#include <DwarfFileSystem/FileSystem.h>

namespace df {
	class DefaultFileSystem : public df::FileSystem {
	public:
		auto GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> override;
		auto OpenFile(const df::StringView& filePath, EFileAccess access) const->df::File* override;
		void CloseFile(df::File* file) const override;
		bool CreateDirectory(const df::StringView& dirPath) const override;
	};
}