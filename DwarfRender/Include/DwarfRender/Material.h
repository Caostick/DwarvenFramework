#pragma once

#include <DwarfRender/MaterialRule.h>
#include <DwarfRender/ParamSet.h>

namespace rf {
	class Material {
	public:
		MaterialRuleId m_MaterialRule = nullptr;
		ParamSet* m_Params = nullptr;
	};
}