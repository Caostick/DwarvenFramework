#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
    class Texture;
}

namespace df {
	class InterfaceRenderData {
	public:
        struct Scissor {
            uint32 m_X;
            uint32 m_Y;
            uint32 m_Width;
            uint32 m_Height;

            bool operator == (const Scissor& other) const;
            bool operator != (const Scissor& other) const;
        };

        struct Batch {
            uint32 m_VertexCount;
            Scissor m_Scissor;
            Texture* m_Texture;
        };

        void Reset();

        void Begin(uint32 width, uint32 height);
        void End();

        void SetTexture(Texture* texture);
        void PushScissor(int32 x, int32 y, int32 width, int32 height);
        void PopScissor();

        void PushVertex(const Vec2& position, const Vec2& tc, const Vec4& color);
        void PushVertex(const Vec2& position, const Vec2& tc);
        void PushVertex(const Vec2& position, const Vec4& color);

        auto GetBatches() const -> const Vector<Batch>&;
        auto GetPositions() const -> const Vector<Vec2>&;
        auto GetTexoords() const -> const Vector<Vec2>&;
        auto GetColors() const -> const Vector<Vec4>&;

    public:
        uint32 m_CurrentBufferIndex;
        uint32 m_MappedVertexCount;

        Batch m_CurrentBatch;

        Vector<Scissor> m_ScissorStack;
        Vector<Batch> m_Batches;

        Vector<Vec2> m_Positions;
        Vector<Vec2> m_Texcoords;
        Vector<Vec4> m_Colors;
	};
}