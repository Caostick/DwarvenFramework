#include <DwarfResources/Data.h>

#include <DwarfFileSystem/FileSystem.h>

#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/StringOperations.h>
#include <DwarvenCore/FileNameOperations.h>
#include <DwarvenCore/Assert.h>

namespace {
	auto GetFilePathTokens(const df::StringView& filename) -> df::Vector<df::StringView> {
		df::Vector<df::StringView> tokens;
		for (const auto& token : df::Tokenize(filename, "/")) {
			tokens.push_back(token);
		}
		return tokens;
	}

	auto GetFileNameTokens(const df::StringView& filename) -> df::Vector<df::StringView> {
		df::Vector<df::StringView> tokens;
		for (const auto& token : df::Tokenize(filename, ".")) {
			tokens.push_back(token);
		}
		return tokens;
	}
}

df::Data::Data(const df::FileSystem& fileSystem) 
	: m_FileSystem(fileSystem) {
}

bool df::Data::Parse(const df::StringView& dataPath) {
	struct TmpFileInfo {
		df::String m_Module;
		df::String m_Extension;
	};

	df::Vector<df::StringView> dataPathTokens;

	const auto optimizedPath = df::OptimizePath<char>(dataPath);

	for (auto token : df::Tokenize<char>(optimizedPath, "/")) {
		if (!token.empty()) {
			dataPathTokens.push_back(token);
		}
	}

	df::HashMap<df::String, df::String> modules;
	df::HashMap<df::String, df::String> extensions;
	df::HashMap<df::String, df::Vector<TmpFileInfo>> files;

	df::Vector<df::String> filepathes;

	const auto dirFiles = m_FileSystem.GetFilesRecursive(dataPath);
	for (const auto& file : dirFiles) {

		const df::String filepath = FixSlash<char>(file);
		df::Vector<df::StringView> filePathTokens;
		for (auto token : df::Tokenize(df::StringView(filepath), "/")) {
			filePathTokens.push_back(token);
		}

		if (filePathTokens.size() - dataPathTokens.size() < 2) {
			continue;
		}

		filepathes.emplace_back(FixSlash<char>(file));

		df::Vector<df::StringView> fileNameTokens;
		for (auto token : df::Tokenize(filePathTokens[filePathTokens.size() - 1], ".")) {
			fileNameTokens.push_back(token);
		}

		df::String filePath;
		for (size_t i = dataPathTokens.size() + 1; i < filePathTokens.size() - 1; ++i) {
			filePath += df::String(filePathTokens[i]);
			filePath += "/";
		}
		filePath += df::String(fileNameTokens[0]);

		TmpFileInfo fileInfo;
		fileInfo.m_Module = df::String(filePathTokens[dataPathTokens.size()]);
		modules[fileInfo.m_Module] = fileInfo.m_Module;

		if (fileNameTokens.size() == 2) {
			fileInfo.m_Extension = df::String(fileNameTokens[1]);
			extensions[fileInfo.m_Extension] = fileInfo.m_Extension;
		}

		files[filePath].emplace_back(fileInfo);
	}

	for (const auto& module : modules) {
		m_Modules.push_back(module.second);
	}

	for (const auto& extension : extensions) {
		m_Extensions.push_back(extension.second);
	}

	for (const auto& file : files) {
		df::FileInfo fileInfo;

		for (const auto& instInfo : file.second) {
			df::FileInfo::Instance instance;
			instance.m_ExtensionIndex = -1;
			instance.m_ModuleIndex = -1;

			for (int i = 0; i < m_Modules.size(); ++i) {
				if (m_Modules[i] == instInfo.m_Module) {
					instance.m_ModuleIndex = i;
					break;
				}
			}

			for (int i = 0; i < m_Extensions.size(); ++i) {
				if (m_Extensions[i] == instInfo.m_Extension) {
					instance.m_ExtensionIndex = i;
					break;
				}
			}

			fileInfo.m_Instances.emplace_back(instance);
		}

		fileInfo.m_Filename = file.first;

		m_Files.emplace_back(fileInfo);
	}

	return true;
}

auto df::Data::GetFileCount() const -> uint32 {
	return uint32(m_Files.size());
}

auto df::Data::GetModuleCount() const->uint32 {
	return uint32(m_Modules.size());
}

auto df::Data::GetExtensionCount() const->uint32 {
	return uint32(m_Extensions.size());
}

auto df::Data::GetFile(uint32 index) const -> const df::FileInfo& {
	DFAssert(index < m_Files.size(), "File index out of range!");

	return m_Files[index];
}

auto df::Data::GetModule(uint32 index) const -> const df::String& {
	DFAssert(index < m_Modules.size(), "Module index out of range!");

	return m_Modules[index];
}

auto df::Data::GetExtension(uint32 index) const -> const df::String& {
	DFAssert(index < m_Extensions.size(), "Extension index out of range!");

	return m_Extensions[index];
}