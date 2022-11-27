#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	class FileSystem;
}

namespace df {
	struct FileInfo {
		struct Instance {
			int m_ModuleIndex;
			int m_ExtensionIndex;
		};

		df::String m_Filename;
		df::Vector<Instance> m_Instances;
	};

	class Data {
	public:
		Data(const df::FileSystem& fileSystem);

		bool Parse(const df::StringView& dataPath);

		auto GetFileCount() const -> uint32;
		auto GetModuleCount() const->uint32;
		auto GetExtensionCount() const->uint32;

		auto GetFile(uint32 index) const -> const df::FileInfo&;
		auto GetModule(uint32 index) const -> const df::String&;
		auto GetExtension(uint32 index) const -> const df::String&;

	private:
		const df::FileSystem& m_FileSystem;

		df::Vector<df::String> m_Extensions;
		df::Vector<df::String> m_Modules;
		df::Vector<df::FileInfo> m_Files;
	};
}