#include "ShaderIncludeCache.h"

#include <DwarfFileSystem/FileSystem.h>
#include <DwarfFileSystem/FileData.h>
#include <DwarfFileSystem/File.h>

df::ShaderIncludeCache::ShaderIncludeCache(FileSystem& fileSystem, const StringView& dataPath)
	: m_FileSystem(fileSystem)
	, m_DataPath(String(dataPath)) {

}

auto df::ShaderIncludeCache::Get(const StringView& include) -> const String& {
	const String strInclude = String(include);

	// Read block
	{
		ReadLock rLock(m_LockStorage);
		auto it = m_Includes.find(strInclude);
		if (it != m_Includes.end()) {
			return it->second;
		}
	}

	// Read write block
	{
		WriteLock wLock(m_LockStorage);
		auto file = m_FileSystem.OpenFile(m_DataPath + "/" + strInclude, EFileAccess::Read);
		if (file) {
			const auto data = file->MapData();
			const auto size = data.GetSize();
			const auto bytes = (char*)data.GetData();

			m_Includes[strInclude] = String(bytes, size);
			m_FileSystem.CloseFile(file);

			return m_Includes[strInclude];
		}
	}

	return m_Empty;
}