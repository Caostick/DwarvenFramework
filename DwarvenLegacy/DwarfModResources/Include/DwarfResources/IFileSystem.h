#pragma once

#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>

namespace df {
	class IFile;
}

namespace df {
	enum class EFileAccess {
		Read,
		Write
	};

	class IFileSystem {
	public:
		virtual ~IFileSystem() {}

		virtual auto GetFilesRecursive(const df::StringView& directory) const->df::Vector<df::String> = 0;
		virtual auto OpenFile(const df::StringView& filePath, EFileAccess access) const -> df::IFile* = 0;
		virtual void CloseFile(df::IFile* file) const = 0;
		virtual bool CreateDirectory(const df::StringView& dirPath) const = 0;
	};
}