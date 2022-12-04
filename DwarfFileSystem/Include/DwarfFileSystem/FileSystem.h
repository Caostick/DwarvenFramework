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

		virtual auto GetFilesRecursive(const StringView& directory) const->Vector<String> = 0;
		virtual auto OpenFile(const StringView& filePath, EFileAccess access) const->File* = 0;
		virtual void CloseFile(File* file) const = 0;
		virtual bool CreateDirectory(const StringView& dirPath) const = 0;
		virtual auto GetUserLocalDirectory() const->String = 0;
	};
}