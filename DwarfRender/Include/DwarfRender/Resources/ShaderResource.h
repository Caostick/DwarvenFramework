#pragma once

#include <DwarfRender/ObjectId.h>

#include <DwarfResources/Resource.h>

namespace rf {
	class ShaderResource : public df::IResource {};

	class VertexShaderResource : public rf::ShaderResource {
	public:
		rf::VertexShaderModuleId m_ShaderModuleId;
	};

	class FragmentShaderResource : public rf::ShaderResource {
	public:
		rf::FragmentShaderModuleId m_ShaderModuleId;
	};

	class ComputeShaderResource : public rf::ShaderResource {
	public:
		rf::ComputeShaderModuleId m_ShaderModuleId;
	};
}