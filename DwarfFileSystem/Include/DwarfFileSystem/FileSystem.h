#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>

namespace df {
	class File;
}

namespace df {
	enum class EFileAccess { Read, Write };

	class FileSystem {
	public:
		virtual ~FileSystem() {}

		virtual auto GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> = 0;
		virtual auto OpenFile(const df::StringView& filePath, EFileAccess access) const->df::File* = 0;
		virtual void CloseFile(df::File* file) const = 0;
		virtual bool CreateDirectory(const df::StringView& dirPath) const = 0;
	};
}