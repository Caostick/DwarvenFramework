#pragma once

#include <DwarfRender/ObjectId.h>

#include <DwarvenCore/Types.h>
#include <DwarvenCore/String.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/HashMap.h>
#include <DwarvenCore/Math/Math.h>

namespace rf {
	class ModelStream {
	public:
		struct Batch {
			rf::MeshId m_Mesh = nullptr;
			rf::MaterialId m_Material = nullptr;

			uint32 m_InstanceCount = 0;
		};

		ModelStream();

		void Reset();

		void SetMesh(rf::MeshId mesh);
		void SetMaterial(rf::MaterialId material);

		// @TODO: 
		void Push();

		auto GetBatches() const ->const df::Vector<Batch>&;

	protected:
		void AddBatch();

		df::Vector<Batch> m_Batches;
		Batch* m_CurrentBatch;
	};
}