#pragma once

#include <DwarfResources/ResourceLoader.h>

namespace rf {
	class ShaderIncludeLoader : public df::IResourceLoader {
	public:
		auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* override;
		void LoadResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size, df::IResource* resource) const override;
		void UnloadResource(df::IResource* resource) const override;
		auto SaveResource(const df::IResource* resource) const->df::Vector<uint8> override;
		auto GetResourceExtension() const->df::String override;
	};
}
