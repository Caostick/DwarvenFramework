#pragma once

#include <DwarfResources/Resource.h>

#include <DwarfRender/ObjectId.h>

namespace rf {
	class MaterialResource : public df::IResource {
	public:
		MaterialId m_Material = nullptr;
	};
}