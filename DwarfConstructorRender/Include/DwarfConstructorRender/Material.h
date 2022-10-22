#pragma once

#include <DwarfConstructorRender/MaterialRule.h>
#include <DwarfConstructorRender/ParamSet.h>

namespace rf {
	class Material {
	public:
		MaterialRuleId m_MaterialRule = nullptr;
		ParamSet* m_Params = nullptr;
	};
}