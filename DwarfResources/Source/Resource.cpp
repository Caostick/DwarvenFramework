#include <DwarfResources/Resource.h>

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

const df::ResourceId df::ResourceId::Unassigned = df::ResourceId({ 0, 0, 0, 0, 0 });
const df::ResourceId df::ResourceId::Invalid = df::ResourceId({ 0, 1, 0, 0, 0 });
