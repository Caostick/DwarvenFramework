#include <DwarfRender/ModelStream.h>

#include <DwarvenCore/Assert.h>

rf::ModelStream::ModelStream() {
	Reset();
}

void rf::ModelStream::Reset() {
	m_Batches.clear();
	m_Batches.push_back(Batch());
	m_CurrentBatch = &m_Batches.back();
}

void rf::ModelStream::SetMesh(rf::MeshId mesh) {
	DFAssert(m_CurrentBatch, "Batch is not set!");

	if (m_CurrentBatch->m_Mesh == mesh) {
		return;
	}

	if (m_CurrentBatch->m_InstanceCount != 0) {
		AddBatch();
	}

	m_CurrentBatch->m_Mesh = mesh;
}

void rf::ModelStream::SetMaterial(rf::MaterialId material) {
	DFAssert(m_CurrentBatch, "Batch is not set!");

	if (m_CurrentBatch->m_Material == material) {
		return;
	}

	if (m_CurrentBatch->m_InstanceCount != 0) {
		AddBatch();
	}

	m_CurrentBatch->m_Material = material;
}

void rf::ModelStream::Push() {
	DFAssert(m_CurrentBatch, "Batch is not set!");

	m_CurrentBatch->m_InstanceCount++;
}

auto rf::ModelStream::GetBatches() const ->const df::Vector<Batch>& {
	return m_Batches;
}

void rf::ModelStream::AddBatch() {
	DFAssert(m_CurrentBatch, "Batch is not set!");

	Batch batch;
	batch.m_InstanceCount = 0;
	batch.m_Mesh = m_CurrentBatch->m_Mesh;
	batch.m_Material = m_CurrentBatch->m_Material;

	m_Batches.emplace_back(batch);
	m_CurrentBatch = &m_Batches.back();
}