#pragma once

#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Lock.h>

namespace df {
	class FileSystem;
}

namespace df {
	class ShaderIncludeCache {
	public:
		ShaderIncludeCache(FileSystem& fileSystem, const StringView& dataPath);

		auto Get(const StringView& include) -> const String&;

	private:
		FileSystem& m_FileSystem;
		String m_DataPath;

		HashMap<String, String> m_Includes;
		String m_Empty;

		Lock m_LockStorage;
	};
}