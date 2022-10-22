#pragma once

#include <DwarfResources/ResourceLoader.h>

namespace rf {
	class Renderer;
}

namespace rf {
	class MeshLoader : public df::IResourceLoader {
	public:
		struct AttributeRedirect {
			int32 PositionAttributeIdx = -1;
			int32 Texcoord0AttributeIdx = -1;
			int32 Texcoord1AttributeIdx = -1;
			int32 Texcoord2AttributeIdx = -1;
			int32 Texcoord3AttributeIdx = -1;
			int32 NormalAttributeIdx = -1;
			int32 BinormalAttributeIdx = -1;
			int32 TangentAttributeIdx = -1;
			int32 QTangentAttributeIdx = -1;
		};

	public:
		MeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer);

		auto MapResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size) const->df::IResource* override;
		void LoadResource(const df::ResourceManager& resourceManager, const df::String& name, void* data, size_t size, df::IResource* resource) const override;
		void UnloadResource(df::IResource* resource) const override;
		auto SaveResource(const df::IResource* resource) const->df::Vector<uint8> override;

	private:
		rf::Renderer& m_Renderer;
		AttributeRedirect m_AttributeRedirect;
	};

	class FbxMeshLoader : public MeshLoader {
	public:
		FbxMeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer);

		auto GetResourceExtension() const->df::String override;
	};

	class ObjMeshLoader : public MeshLoader {
	public:
		ObjMeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer);

		auto GetResourceExtension() const->df::String override;
	};
}