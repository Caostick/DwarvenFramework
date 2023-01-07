#pragma once 

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Math/Math.h>

namespace df {
	class InterfaceRenderData;
	class Texture;
	class Font;
}

namespace df {
	class InterfaceRenderer {
	public:
		InterfaceRenderer(InterfaceRenderData& renderData);

		void SetScreenSize(uint32 width, uint32 height);

		void SetTexture(Texture* texture);
		void PushClipRect(int32 x, int32 y, uint32 width, uint32 height);
		void PopClipRect();

		void DrawPolygon(const Vec2* points, uint32 numPoints, const Vec4& color);
		void DrawPolygon(const Vec2* points, const Vec2* texcoords, uint32 numPoints, const Vec4& color);
		void DrawSolidCircle(float x, float y, float width, float height, const Vec4& color);
		void DrawSolidQuad(float x, float y, float width, float height, const Vec2& tcMins, const Vec2& tcMaxs, const Vec4& color);
		void DrawSolidQuad(float x, float y, float width, float height, const Vec4& color);
		void DrawGradientQuad(float x, float y, float width, float height, const Vec4& color0, const Vec4& color1, const Vec4& color2, const Vec4& color3);
		void DrawLine(float x1, float y1, float x2, float y2, float width, const Vec4& color);

		void DrawText(float x, float y, const char* text, uint32 length, const Font& font, const Vec4& color, float scale = 1.0f);
		void DrawText(float x, float y, const char* text, const Font& font, const Vec4& color, float scale = 1.0f);
		void DrawText(float x, float y, const wchar_t* text, uint32 length, const Font& font, const Vec4& color, float scale = 1.0f);
		void DrawText(float x, float y, const wchar_t* text, const Font& font, const Vec4& color, float scale = 1.0f);

	private:
		InterfaceRenderData& m_RenderData;

		float m_ScreenWidth;
		float m_ScreenHeight;
		float m_InvDesignWidth;
		float m_InvDesignHeight;
	};
}