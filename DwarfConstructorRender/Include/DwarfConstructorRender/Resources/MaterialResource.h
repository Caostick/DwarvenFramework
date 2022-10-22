#pragma once

#include <DwarfResources/Resource.h>

#include <DwarfConstructorRender/ObjectId.h>

namespace rf {
	class MaterialResource : public df::IResource {
	public:
		MaterialId m_Material = nullptr;
	};
}