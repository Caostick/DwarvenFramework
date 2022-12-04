#pragma once

template<typename ResourceType>
bool df::TResourceLoader<ResourceType>::VisitResource(ResourceType::Data& /*resourceData*/, DataVisitor& /*visitor*/) {
	return false;
}

template<typename ResourceType>
bool df::TResourceLoader<ResourceType>::Load(void* resourceDataPtr, const StringView& path, const StringView& guid, void* data, size_t size) {
	typename ResourceType::Data* resourceData = reinterpret_cast<ResourceType::Data*>(resourceDataPtr);

	return Load(*resourceData, path, guid, data, size);
}

template<typename ResourceType>
bool df::TResourceLoader<ResourceType>::VisitResource(void* resourceDataPtr, DataVisitor& visitor) {
	typename ResourceType::Data* resourceData = reinterpret_cast<ResourceType::Data*>(resourceDataPtr);

	return VisitResource(*resourceData, visitor);
}