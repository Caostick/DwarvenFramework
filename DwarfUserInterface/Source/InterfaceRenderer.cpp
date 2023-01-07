#include<DwarfUserInterface/InterfaceRenderer.h>
#include<DwarfUserInterface/Font.h>

#include "InterfaceRenderData.h"

df::InterfaceRenderer::InterfaceRenderer(InterfaceRenderData& renderData) 
	: m_RenderData(renderData) 
	, m_ScreenWidth(1.0f)
	, m_ScreenHeight(1.0f)
	, m_InvDesignWidth(1.0f)
	, m_InvDesignHeight(1.0f) {

}

void df::InterfaceRenderer::SetScreenSize(uint32 width, uint32 height) {
	m_ScreenWidth = float(width);
	m_ScreenHeight = float(height);
	m_InvDesignWidth = 1.0f / m_ScreenWidth;
	m_InvDesignHeight = 1.0f / m_ScreenHeight;
}

void df::InterfaceRenderer::SetTexture(Texture* texture) {
	m_RenderData.SetTexture(texture);
}

void df::InterfaceRenderer::PushClipRect(int32 x, int32 y, uint32 width, uint32 height) {
	m_RenderData.PushScissor(x, y, int32(width), int32(height));
}

void df::InterfaceRenderer::PopClipRect() {
	m_RenderData.PopScissor();
}

void df::InterfaceRenderer::DrawPolygon(
	const Vec2* points,
	uint32 numPoints,
	const Vec4& color) {

	Vec2 vPosition;
	Vec2 vTexcoord = Vec2(0.0f, 0.0f);

	const Vec2 invScrScale = Vec2(m_InvDesignWidth, m_InvDesignHeight);
	for(uint32 i = 1; i <= numPoints - 2; ++i) {

		vPosition = points[0] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);

		vPosition = points[i + 1] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);

		vPosition = points[i] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);
	}

	//@TODO: FeatherPolygon
}

void df::InterfaceRenderer::DrawPolygon(
	const Vec2* points,
	const Vec2* texcoords,
	uint32 numPoints,
	const Vec4& color) {

	Vec2 vPosition;
	Vec2 vTexcoord = Vec2(0.0f, 0.0f);

	const Vec2 invScrScale = Vec2(m_InvDesignWidth, m_InvDesignHeight);
	for(uint32 i = 1; i <= numPoints - 2; ++i) {

		vTexcoord = texcoords[0];
		vPosition = points[0] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);

		vTexcoord = texcoords[i + 1];
		vPosition = points[i + 1] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);

		vTexcoord = texcoords[i];
		vPosition = points[i] * invScrScale;
		m_RenderData.PushVertex(vPosition, vTexcoord, color);
	}

	//@TODO: FeatherPolygon
}

void df::InterfaceRenderer::DrawSolidCircle(
	float x,
	float y,
	float width,
	float height,
	const Vec4& color) {

	const Vec2 points[12] = {
		Vec2(1.000000, 0.500000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.933013, 0.750000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.750000, 0.933013) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.500000, 1.000000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.250000, 0.933013) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.066987, 0.750000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.000000, 0.500000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.066987, 0.250000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.250000, 0.066987) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.500000, 0.000000) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.750000, 0.066987) * Vec2(width, height) + Vec2(x, y),
		Vec2(0.933013, 0.250000) * Vec2(width, height) + Vec2(x, y)
	};

	DrawPolygon(points, 12, color);
}

void df::InterfaceRenderer::DrawSolidQuad(
	float x,
	float y,
	float width,
	float height,
	const Vec2& tcMins,
	const Vec2& tcMaxs,
	const Vec4& color) {

	const Vec2 points[4] = {
		Vec2(x, y),
		Vec2(x + width, y),
		Vec2(x + width, y + height),
		Vec2(x, y + height),
	};

	const Vec2 texcoords[4] = {
		Vec2(tcMins.X, tcMins.Y),
		Vec2(tcMaxs.X, tcMins.Y),
		Vec2(tcMaxs.X, tcMaxs.Y),
		Vec2(tcMins.X, tcMaxs.Y)
	};

	DrawPolygon(points, texcoords, 4, color);
}

void df::InterfaceRenderer::DrawSolidQuad(
	float x,
	float y,
	float width,
	float height,
	const Vec4& color) {

	DrawSolidQuad(x, y, width, height, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), color);
}

void df::InterfaceRenderer::DrawGradientQuad(
	float x,
	float y,
	float width,
	float height,
	const Vec4& color0,
	const Vec4& color1,
	const Vec4& color2,
	const Vec4& color3) {

	const Vec2 invScrScale = Vec2(m_InvDesignWidth, m_InvDesignHeight);

	Vec2 vPosition;
	Vec4 vColor;

	vPosition = Vec2(x, y + height) * invScrScale;
	vColor = color3;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);

	vPosition = Vec2(x + width, y) * invScrScale;
	vColor = color1;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);

	vPosition = Vec2(x, y) * invScrScale;
	vColor = color0;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);

	vPosition = Vec2(x + width, y + height) * invScrScale;
	vColor = color2;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);

	vPosition = Vec2(x + width, y) * invScrScale;
	vColor = color1;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);

	vPosition = Vec2(x, y + height) * invScrScale;
	vColor = color3;
	m_RenderData.PushVertex(vPosition, Vec2(0.0f, 0.0f), vColor);
}

void df::InterfaceRenderer::DrawLine(
	float x1,
	float y1,
	float x2,
	float y2,
	float width,
	const Vec4& color) {

	const Vec2 p1 = Vec2(x1, y1);
	const Vec2 p2 = Vec2(x2, y2);
	const Vec2 tangent = Vec2::Normalized(p2 - p1);
	const Vec2 normal = Vec2(-tangent.Y, tangent.X) * (width * 0.5f);
	const Vec2 points[4] = {
		p1 - normal,
		p2 - normal,
		p2 + normal,
		p1 + normal
	};

	DrawPolygon(points, 4, color);
}

void df::InterfaceRenderer::DrawText(
	float x, 
	float y,
	const char* text,
	uint32 length, 
	const Font& font,
	const Vec4& color,
	float scale /*= 1.0f*/) {

	float xCursor = float(x);
	float yCursor = float(y);

	const float invTexWidth = font.GetInvTextureWidth();
	const float invTexHeight = font.GetInvTextureHeight();

	m_RenderData.SetTexture(font.GetTexture());
	Vec2 vPositions[4];
	Vec2 vTexcoords[4];

	for(uint32 i = 0; i < length; ++i) {
		const char symbol = text[i];
		const Font::Glyph& glyph = font[symbol];

		const float minX = float(xCursor + glyph.XOffset * scale);
		const float minY = float(yCursor + glyph.YOffset * scale);
		const float maxX = float(minX + glyph.Width * scale);
		const float maxY = float(minY + glyph.Height * scale);
		const float u0 = glyph.U0 * invTexWidth;
		const float u1 = glyph.U1 * invTexWidth;
		const float v0 = glyph.V0 * invTexHeight;
		const float v1 = glyph.V1 * invTexHeight;

		vPositions[0] = Vec2(minX * m_InvDesignWidth, minY * m_InvDesignHeight);
		vTexcoords[0] = Vec2(u0, v0);

		vPositions[1] = Vec2(maxX * m_InvDesignWidth, minY * m_InvDesignHeight);
		vTexcoords[1] = Vec2(u1, v0);

		vPositions[2] = Vec2(maxX * m_InvDesignWidth, maxY * m_InvDesignHeight);
		vTexcoords[2] = Vec2(u1, v1);

		vPositions[3] = Vec2(minX * m_InvDesignWidth, maxY * m_InvDesignHeight);
		vTexcoords[3] = Vec2(u0, v1);

		m_RenderData.PushVertex(vPositions[2], vTexcoords[2], color);
		m_RenderData.PushVertex(vPositions[1], vTexcoords[1], color);
		m_RenderData.PushVertex(vPositions[0], vTexcoords[0], color);
		m_RenderData.PushVertex(vPositions[3], vTexcoords[3], color);
		m_RenderData.PushVertex(vPositions[2], vTexcoords[2], color);
		m_RenderData.PushVertex(vPositions[0], vTexcoords[0], color);

		xCursor += glyph.XAdvance * scale;
	}
}

void df::InterfaceRenderer::DrawText(
	float x,
	float y,
	const char* text,
	const Font& font,
	const Vec4& color,
	float scale /*= 1.0f*/) {

	DrawText(x, y, text, uint32(strlen(text)), font, color, scale);

}

void df::InterfaceRenderer::DrawText(
	float x,
	float y,
	const wchar_t* text,
	uint32 length,
	const Font& font,
	const Vec4& color,
	float scale /*= 1.0f*/) {

	float xCursor = float(x);
	float yCursor = float(y);

	const float invTexWidth = font.GetInvTextureWidth();
	const float invTexHeight = font.GetInvTextureHeight();

	m_RenderData.SetTexture(font.GetTexture());
	Vec2 vPositions[4];
	Vec2 vTexcoords[4];

	for(uint32 i = 0; i < length; ++i) {
		const wchar_t symbol = text[i];
		const Font::Glyph& glyph = font[symbol];

		const float minX = float(xCursor + glyph.XOffset * scale);
		const float minY = float(yCursor + glyph.YOffset * scale);
		const float maxX = float(minX + glyph.Width * scale);
		const float maxY = float(minY + glyph.Height * scale);
		const float u0 = glyph.U0 * invTexWidth;
		const float u1 = glyph.U1 * invTexWidth;
		const float v0 = glyph.V0 * invTexHeight;
		const float v1 = glyph.V1 * invTexHeight;

		vPositions[0] = Vec2(minX * m_InvDesignWidth, minY * m_InvDesignHeight);
		vTexcoords[0] = Vec2(u0, v0);

		vPositions[1] = Vec2(maxX * m_InvDesignWidth, minY * m_InvDesignHeight);
		vTexcoords[1] = Vec2(u1, v0);

		vPositions[2] = Vec2(maxX * m_InvDesignWidth, maxY * m_InvDesignHeight);
		vTexcoords[2] = Vec2(u1, v1);

		vPositions[3] = Vec2(minX * m_InvDesignWidth, maxY * m_InvDesignHeight);
		vTexcoords[3] = Vec2(u0, v1);

		m_RenderData.PushVertex(vPositions[2], vTexcoords[2], color);
		m_RenderData.PushVertex(vPositions[1], vTexcoords[1], color);
		m_RenderData.PushVertex(vPositions[0], vTexcoords[0], color);
		m_RenderData.PushVertex(vPositions[3], vTexcoords[3], color);
		m_RenderData.PushVertex(vPositions[2], vTexcoords[2], color);
		m_RenderData.PushVertex(vPositions[0], vTexcoords[0], color);

		xCursor += glyph.XAdvance * scale;
	}
}

void df::InterfaceRenderer::DrawText(
	float x,
	float y,
	const wchar_t* text,
	const Font& font,
	const Vec4& color,
	float scale /*= 1.0f*/) {

	DrawText(x, y, text, uint32(wcslen(text)), font, color, scale);
}