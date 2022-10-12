#pragma once

#include <DwarfRender/Loaders/ShaderCompiler.h>

#include <DwarfResources/ResourceLoader.h>

namespace rf {
	class Renderer;
}

namespace rf {
	class ShaderCodeLoader : public df::IResourceLoader {
	public:
		ShaderCodeLoader(rf::Renderer& renderer);
		virtual ~ShaderCodeLoader();

		void LoadResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size, df::IResource* resource) const override;
		auto SaveResource(const df::IResource* resource) const->df::Vector<uint8> override;

	protected:
		virtual auto GetShaderType() const->rf::EShaderType = 0;

		auto LoadShader(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const -> rf::ShaderCompileInfo;
		bool ProcessLog() const;

	protected:
		rf::ShaderCompiler* m_ShaderCompiler;
		rf::Renderer& m_Renderer;
	};

	class VertexShaderCodeLoader : public ShaderCodeLoader {
	public:
		VertexShaderCodeLoader(rf::Renderer& renderer);

		auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* override;
		void UnloadResource(df::IResource* resource) const override;
		auto GetResourceExtension() const->df::String override;

	protected:
		auto GetShaderType() const ->rf::EShaderType override;
	};

	class FragmentShaderCodeLoader : public ShaderCodeLoader {
	public:
		FragmentShaderCodeLoader(rf::Renderer& renderer);

		auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* override;
		void UnloadResource(df::IResource* resource) const override;
		auto GetResourceExtension() const->df::String override;

	protected:
		auto GetShaderType() const->rf::EShaderType override;
	};

	class ComputeShaderCodeLoader : public ShaderCodeLoader {
	public:
		ComputeShaderCodeLoader(rf::Renderer& renderer);

		auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* override;
		void UnloadResource(df::IResource* resource) const override;
		auto GetResourceExtension() const->df::String override;

	protected:
		auto GetShaderType() const->rf::EShaderType override;
	};
}