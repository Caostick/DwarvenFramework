#pragma once

#include <DwarfResources/Resource.h>

#include <DwarfRender/ObjectId.h>

namespace rf {
	class MaterialRuleResource : public df::IResource {
	public:
		MaterialRuleId m_MaterialRule = nullptr;
	};
}