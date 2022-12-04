#pragma once

#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>
#include <DwarvenCore/DataVisitor.h>


namespace df {
	class ResourceManager;
	class IResource;
}

namespace df {
	class ResourceLoader {
	public:
		virtual ~ResourceLoader() {}

		virtual bool Load(void* resourceDataPtr, const StringView& path, const StringView& guid, void* data, size_t size) = 0;
		virtual bool VisitResource(void* resourceDataPtr, DataVisitor& visitor) = 0;
	};

	template<typename ResourceType>
	class TResourceLoader : public ResourceLoader {
	public:
		virtual bool Load(ResourceType::Data& resourceData, const StringView& path, const StringView& guid, void* data, size_t size) = 0;
		virtual bool VisitResource(ResourceType::Data& resourceData, DataVisitor& visitor);
		virtual auto GetResourceExtension() const -> String = 0;

	public:
		bool Load(void* resourceDataPtr, const StringView& path, const StringView& guid, void* data, size_t size) override;
		bool VisitResource(void* resourceDataPtr, DataVisitor& visitor) override;
	};
}

#include <DwarfResources/ResourceLoader.inl>