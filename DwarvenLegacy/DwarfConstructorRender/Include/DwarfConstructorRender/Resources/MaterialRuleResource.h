#pragma once

#include <DwarfResources/Resource.h>

#include <DwarfConstructorRender/ObjectId.h>

namespace rf {
	class MaterialRuleResource : public df::IResource {
	public:
		MaterialRuleId m_MaterialRule = nullptr;
	};
}