#include <DwarfResources/Resource.h>

namespace {
	auto MakeUnassigned() ->df::ResourceId {
		df::ResourceId id = {};

		id.m_IsValid = 0;
		id.m_IsAssigned = 0;
		id.m_ModuleSet = 0;
		id.m_ResourceTypeId  = 0;
		id.m_ResourceIndex = 0;

		return id;
	}

	auto MakeInvalid() ->df::ResourceId {
		df::ResourceId id = {};

		id.m_IsValid = 0;
		id.m_IsAssigned = 1;
		id.m_ModuleSet = 0;
		id.m_ResourceTypeId = 0;
		id.m_ResourceIndex = 0;

		return id;
	}
}

int df::ResourceCapacity::s_ResourceTypeCount = 0;

auto df::ResourceId::operator==(const ResourceId& other) const -> bool {
	return
		(m_IsValid == other.m_IsValid) &&
		(m_IsAssigned == other.m_IsAssigned) &&
		(m_ModuleSet == other.m_ModuleSet) &&
		(m_ResourceTypeId == other.m_ResourceTypeId) &&
		(m_ResourceIndex == other.m_ResourceIndex);
}

auto df::ResourceId::operator!=(const ResourceId& other) const -> bool {
	return
		(m_IsValid != other.m_IsValid) ||
		(m_IsAssigned != other.m_IsAssigned) ||
		(m_ModuleSet != other.m_ModuleSet) ||
		(m_ResourceTypeId != other.m_ResourceTypeId) ||
		(m_ResourceIndex != other.m_ResourceIndex);
}

const df::ResourceId df::ResourceId::Unassigned = MakeUnassigned();
const df::ResourceId df::ResourceId::Invalid = MakeInvalid();
