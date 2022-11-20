#include <DwarfResources/ResourceManager.h>
#include <DwarfResources/Data.h>
#include <DwarfResources/IFileSystem.h>
#include <DwarfResources/IFile.h>


#include <assert.h>

df::ResourceManager::ResourceManager(const df::IFileSystem& fileSystem, const df::StringView& dataPath)
	: m_FileSystem(fileSystem) 
	, m_DataPath(dataPath) 
	, m_ActiveModuleSetIndex(0)
{
	m_ModuleSets.emplace_back(df_private::ActiveModuleSet());
}

df::ResourceManager::~ResourceManager() {
	for (int i = 0; i < m_ResourceTypeContainers.size(); ++i) {
		if (m_ResourceTypeContainers[i]) {
			DFDelete m_ResourceTypeContainers[i];
			m_ResourceTypeContainers[i] = nullptr;
		}
	}
}

void df::ResourceManager::SetupModules(const df::Vector<df::String>& activeModules) {
	m_ActiveModuleSetIndex = -1;
	for (int i = 0; i < m_ModuleSets.size(); ++i) {
		if (m_ModuleSets[i].m_ActiveModules == activeModules) {
			m_ActiveModuleSetIndex = i;
		}
	}

	if (m_ActiveModuleSetIndex == -1) {
		m_ModuleSets.emplace_back(df_private::ActiveModuleSet({ activeModules }));
		m_ActiveModuleSetIndex = int(m_ModuleSets.size()) - 1;
	}
}

void df::ResourceManager::ParseData() {
	Data data(m_FileSystem);
	data.Parse(m_DataPath);

	const uint32 fileCount = data.GetFileCount();
	for (uint32 i = 0; i < fileCount; ++i) {
		const auto& file = data.GetFile(i);

		if (file.m_Filename.empty()) {
			continue;
		}

		for (const auto& inst : file.m_Instances) {
			if (inst.m_ExtensionIndex == -1) {
				continue;
			}

			if (inst.m_ModuleIndex == -1) {
				continue;
			}

			const auto& extension = data.GetExtension(inst.m_ExtensionIndex);
			const auto& moduleName = data.GetModule(inst.m_ModuleIndex);

			PushResource(file.m_Filename, moduleName, extension);
		}
	}
}

void df::ResourceManager::UnloadResources() {
	for (int i = 0; i < m_ResourceTypeContainers.size(); ++i) {
		if (m_ResourceTypeContainers[i]) {
			m_ResourceTypeContainers[i]->UnloadResources();
		}
	}
}

void df::ResourceManager::ResetResources() {
	for (int i = 0; i < m_ResourceTypeContainers.size(); ++i) {
		if (m_ResourceTypeContainers[i]) {
			m_ResourceTypeContainers[i]->ResetResources();
		}
	}
}

void df::ResourceManager::PushResource(const df::StringView& name, const df::StringView& module, const df::StringView& extension) {
	DFAssert(m_ActiveModuleSetIndex > 0, "ModuleSet wasn't set!");

	int moduleIndex = -1;
	for (int i = 0; i < m_ModuleSets[m_ActiveModuleSetIndex].m_ActiveModules.size(); ++i) {
		if (module == m_ModuleSets[m_ActiveModuleSetIndex].m_ActiveModules[i]) {
			moduleIndex = i;
			break;
		}
	}

	if (moduleIndex >= 0) {
		for (int i = 0; i < m_ResourceTypeContainers.size(); ++i) {
			if (m_ResourceTypeContainers[i]) {
				m_ResourceTypeContainers[i]->PushResource(name, extension, moduleIndex);
			}
		}
	}
}

void df::ResourceManager::Update() {

}

void df::ResourceManager::MapResources() {
	for (int resTypeId = 0; resTypeId < m_ResourceTypeContainers.size(); ++resTypeId) {
		MapResourcesByResourceId(resTypeId);
	}
}

void df::ResourceManager::MapResourcesByResourceId(int resTypeId) {
	auto* resTypeContainer = m_ResourceTypeContainers[resTypeId];
	if (!resTypeContainer) {
		return;
	}

	//df::Log("Map [%s] resources:", resTypeContainer->GetTypeName().c_str());

	const auto& loaders = resTypeContainer->GetLoaders();
	auto& resources = resTypeContainer->GetResources();

	for (auto& resourceContainer : resources) {
		DFAssert(!resourceContainer.m_Resource, "Resource already mapped!");

		const auto* loader = loaders[resourceContainer.m_LoaderIndex];
		const auto filename = MakeFilePath(resourceContainer.m_ModuleIndex, resourceContainer.m_Name, loader->GetResourceExtension());
		const auto file = m_FileSystem.OpenFile(filename, df::EFileAccess::Read);
		DFAssert(file, "Can't open resource file!");

		const auto fileData = file->MapData();

		//df::Log("  %s", resourceContainer.m_Name.c_str());
		resourceContainer.m_Resource = loader->MapResource(*this, resourceContainer.m_Name, fileData.GetData(), fileData.GetSize());

		//DFAssert(resourceContainer.m_Resource, "Resource mapping failed!");

		m_FileSystem.CloseFile(file);
	}
}

auto df::ResourceManager::MakeFilePath(int moduleIndex, const df::StringView& name, const df::StringView& extension) ->df::String {
	const auto& moduleName = m_ModuleSets[m_ActiveModuleSetIndex].m_ActiveModules[moduleIndex];
	return 
		m_DataPath +
		moduleName +
		df::String("/") +
		df::String(name) +
		df::String(".") +
		df::String(extension);
}
