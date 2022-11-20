#include "Loaders.h"
#include "Resources.h"

#include <DwarvenCore/New.h>

auto FmtALoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* data, size_t size) const->df::IResource* {
	TestResourceAB* resource = DFNew TestResourceAB;
	resource->SetData((char*)data, (uint32)size);

	return resource;
}

void FmtALoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void FmtALoader::UnloadResource(df::IResource* /*resource*/) const {}

auto FmtALoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto FmtALoader::GetResourceExtension() const->df::String {
	return "fmtA";
}



auto FmtBLoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* data, size_t size) const->df::IResource* {
	TestResourceAB* resource = DFNew TestResourceAB;
	resource->SetData((char*)data, (uint32)size);

	return resource;
}

void FmtBLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void FmtBLoader::UnloadResource(df::IResource* /*resource*/) const {}

auto FmtBLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto FmtBLoader::GetResourceExtension() const->df::String {
	return "fmtB";
}



auto FmtCLoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* data, size_t size) const->df::IResource* {
	TestResourceC* resource = DFNew TestResourceC;
	resource->SetData((char*)data, (uint32)size);

	return resource;
}

void FmtCLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void FmtCLoader::UnloadResource(df::IResource* /*resource*/) const {}

auto FmtCLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

auto FmtCLoader::GetResourceExtension() const->df::String {
	return "fmtC";
}