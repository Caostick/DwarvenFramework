#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>


namespace df {
	class IResource;
	class ResourceManager;
}

namespace df {
	class IResourceLoader {
	public:
		virtual ~IResourceLoader() {}

		virtual auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* = 0;
		virtual void LoadResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size, df::IResource* resource) const = 0;
		virtual void UnloadResource(df::IResource* resource) const = 0;
		virtual auto SaveResource(const df::IResource* resource) const -> df::Vector<uint8> = 0;
		virtual auto GetResourceExtension() const -> df::String = 0;
	};
}