#include "InterfaceRenderData.h"

#include <DwarvenCore/Assert.h>

bool df::InterfaceRenderData::Scissor::operator == (const Scissor& other) const {
    return m_X == other.m_X && m_Y == other.m_Y &&
        m_Width == other.m_Width && m_Height == other.m_Height;
}

bool df::InterfaceRenderData::Scissor::operator != (const Scissor& other) const {
    return m_X != other.m_X || m_Y != other.m_Y ||
        m_Width != other.m_Width || m_Height != other.m_Height;
}


void df::InterfaceRenderData::Reset() {
    m_Positions.clear();
    m_Texcoords.clear();
    m_Colors.clear();

    m_Batches.clear();
    m_ScissorStack.clear();
}

void df::InterfaceRenderData::Begin(uint32 width, uint32 height) {
    Scissor scissor = {};
    scissor.m_X = 0;
    scissor.m_Y = 0;
    scissor.m_Width = width;
    scissor.m_Height = height;

    m_CurrentBatch.m_Scissor = scissor;
    m_CurrentBatch.m_Texture = nullptr;
    m_CurrentBatch.m_VertexCount = 0;

    m_ScissorStack.emplace_back(scissor);
}

void df::InterfaceRenderData::End() {
    m_ScissorStack.pop_back();
    DFAssert(m_ScissorStack.empty(), "Scissor stack should be empty at this point!");

    if(m_CurrentBatch.m_VertexCount != 0) {
        m_Batches.push_back(m_CurrentBatch);
    }
}

void df::InterfaceRenderData::SetTexture(Texture* texture) {
    if(m_CurrentBatch.m_Texture != texture && m_CurrentBatch.m_VertexCount != 0) {
        m_Batches.push_back(m_CurrentBatch);
        m_CurrentBatch.m_VertexCount = 0;
    }

    m_CurrentBatch.m_Texture = texture;
}

void df::InterfaceRenderData::PushScissor(int32 x, int32 y, int32 width, int32 height) {

    const uint32 minX = std::max(uint32(std::max(x, int32(0))), m_CurrentBatch.m_Scissor.m_X);
    const uint32 minY = std::max(uint32(std::max(y, int32(0))), m_CurrentBatch.m_Scissor.m_Y);
    const uint32 maxX = std::min(uint32(x + width), uint32(m_CurrentBatch.m_Scissor.m_X + m_CurrentBatch.m_Scissor.m_Width));
    const uint32 maxY = std::min(uint32(y + height), uint32(m_CurrentBatch.m_Scissor.m_Y + m_CurrentBatch.m_Scissor.m_Height));

    Scissor scissor = {};
    scissor.m_X = minX;
    scissor.m_Y = minY;
    scissor.m_Width = uint16_t(maxX - minX);
    scissor.m_Height = uint16_t(maxY - minY);
    m_ScissorStack.emplace_back(scissor);

    if(m_CurrentBatch.m_Scissor != m_ScissorStack.back() && m_CurrentBatch.m_VertexCount != 0) {
        m_Batches.push_back(m_CurrentBatch);
        m_CurrentBatch.m_VertexCount = 0;
    }

    m_CurrentBatch.m_Scissor = m_ScissorStack.back();
}

void df::InterfaceRenderData::PopScissor() {
    DFAssert(!m_ScissorStack.empty(), "Scissor stack is empty!");
    m_ScissorStack.pop_back();

    if(m_CurrentBatch.m_Scissor != m_ScissorStack.back() && m_CurrentBatch.m_VertexCount != 0) {
        m_Batches.push_back(m_CurrentBatch);
        m_CurrentBatch.m_VertexCount = 0;
    }

    m_CurrentBatch.m_Scissor = m_ScissorStack.back();
}

void df::InterfaceRenderData::PushVertex(const Vec2& position, const Vec2& tc, const Vec4& color) {
    m_Positions.push_back(position);
    m_Texcoords.push_back(tc);
    m_Colors.push_back(color);

    m_CurrentBatch.m_VertexCount++;
}

void df::InterfaceRenderData::PushVertex(const Vec2& position, const Vec2& tc) {
    PushVertex(position, tc, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void df::InterfaceRenderData::PushVertex(const Vec2& position, const Vec4& color) {
    PushVertex(position, Vec2(0.0f, 0.0f), color);
}

auto df::InterfaceRenderData::GetBatches() const -> const Vector<Batch>& {
    return m_Batches;
}

auto df::InterfaceRenderData::GetPositions() const -> const Vector<Vec2>& {
    return m_Positions;
}

auto df::InterfaceRenderData::GetTexoords() const -> const Vector<Vec2>& {
    return m_Texcoords;
}

auto df::InterfaceRenderData::GetColors() const -> const Vector<Vec4>& {
    return m_Colors;
}