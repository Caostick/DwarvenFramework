#include <DwarfRender/Loaders/ShaderIncludeLoader.h>
#include <DwarfRender/Resources/ShaderIncludeResource.h>

auto rf::ShaderIncludeLoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* data, size_t size) const->df::IResource* {
	rf::ShaderIncludeResource* resource = DFNew rf::ShaderIncludeResource;

	const auto code = df::String((const char*)data, size);
	resource->SetCode(code);

	return resource;
}

void rf::ShaderIncludeLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void rf::ShaderIncludeLoader::UnloadResource(df::IResource* /*resource*/) const {

}

auto rf::ShaderIncludeLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto rf::ShaderIncludeLoader::GetResourceExtension() const->df::String {
	return "shi";
}