#include <DwarfConstructorRender/Loaders/MeshLoader.h>


#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <DwarfConstructorRender/GlobalObjects.h>
#include <DwarfConstructorRender/Renderer.h>

#include <DwarfConstructorRender/Resources/MeshResource.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/DebugName.h>
#include <DwarvenCore/Math/Math.h>
#include <DwarvenCore/Math/TangentFrame.h>

rf::MeshLoader::MeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer)
	: m_Renderer(renderer) 
	, m_AttributeRedirect(attribRedirect)
{}

auto rf::MeshLoader::MapResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* data, size_t size) const->df::IResource* {
	const aiScene* scene = aiImportFileFromMemory((const char*)data, (unsigned int)size, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipWindingOrder | aiProcess_FlipUVs, nullptr);
	if (!scene) {
		return nullptr;
	}

	df::Vector<Vec3> positions;
	df::Vector<Vec4> qTangents;
	df::Vector<Vec2> texcoords;
	df::Vector<uint32> indices;

	for (uint32 mIdx = 0; mIdx < scene->mNumMeshes; ++mIdx) {
		const aiMesh* mesh = scene->mMeshes[mIdx];

		for (uint32 vIdx = 0; vIdx < mesh->mNumVertices; ++vIdx) {

			const Vec3 position = Vec3(
				mesh->mVertices[vIdx].x,
				mesh->mVertices[vIdx].y,
				mesh->mVertices[vIdx].z);

			const Vec2 texcoord = Vec2(
				mesh->mTextureCoords[0][vIdx].x,
				mesh->mTextureCoords[0][vIdx].y);

			const Vec3 tangent = Vec3(
				mesh->mTangents[vIdx].x,
				mesh->mTangents[vIdx].y,
				mesh->mTangents[vIdx].z);

			const Vec3 binormal = Vec3(
				mesh->mBitangents[vIdx].x,
				mesh->mBitangents[vIdx].y,
				mesh->mBitangents[vIdx].z);

			const Vec3 normal = Vec3(
				mesh->mNormals[vIdx].x,
				mesh->mNormals[vIdx].y,
				mesh->mNormals[vIdx].z);

			const Vec4 qTangent = ComputeQTangentFromTangentFrame(tangent, binormal, normal);

			positions.emplace_back(position);
			qTangents.emplace_back(qTangent);
			texcoords.emplace_back(texcoord);
		}

		for (uint32 fIdx = 0; fIdx < mesh->mNumFaces; ++fIdx) {
			const aiFace& face = mesh->mFaces[fIdx];
			if (face.mNumIndices != 3) {
				DFAssert(false, "Invalid face!");
				continue;
			}

			for (uint32 iIdx = 0; iIdx < face.mNumIndices; ++iIdx) {
				indices.push_back(face.mIndices[iIdx]);
			}
		}
	}

	aiReleaseImport(scene);

	/*const rf::MeshId meshId = m_RenderObjectManager.CreateMesh(
		df::DebugName("\"%s\"", name.c_str()),
		static_cast<uint32>(indices.size()),
		static_cast<uint32>(positions.size()),
		indices.data(),
		positions.data(),
		texcoords.data(),
		qTangents.data()
	);*/

	rf::MeshResource* meshResource = DFNew rf::MeshResource;
	//meshResource->SetMeshId(meshId);

	return meshResource;
}

void rf::MeshLoader::LoadResource(const df::ResourceManager& /*resourceManager*/, const df::String& /*name*/, void* /*data*/, size_t /*size*/, df::IResource* /*resource*/) const {

}

void rf::MeshLoader::UnloadResource(df::IResource* resource) const {
	[[maybe_unused]] rf::MeshResource* meshResource = reinterpret_cast<rf::MeshResource*>(resource);

	/*if (meshResource->GetMeshId() != rf::GlobalObjects::Get(rf::EMesh::Null)) {
		m_RenderObjectManager.DestroyMesh(meshResource->GetMeshId());
	}

	meshResource->SetMeshId(rf::MeshId());*/
}

auto rf::MeshLoader::SaveResource(const df::IResource* /*resource*/) const->df::Vector<uint8> {
	return df::Vector<uint8>();
}

rf::FbxMeshLoader::FbxMeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer)
	: MeshLoader(attribRedirect, renderer) {}

auto rf::FbxMeshLoader::GetResourceExtension() const->df::String {
	return "fbx";
}

rf::ObjMeshLoader::ObjMeshLoader(const AttributeRedirect& attribRedirect, rf::Renderer& renderer)
	: MeshLoader(attribRedirect, renderer) {}

auto rf::ObjMeshLoader::GetResourceExtension() const->df::String {
	return "obj";
}