#include <DwarfUserInterface/UserInterface.h>
#include <DwarfUserInterface/InterfaceRenderer.h>

#include <DwarfRender/Renderer.h>
#include <DwarfRender/Texture.h>

#include <DwarvenCore/New.h>

#include "InterfaceRenderData.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

df::UserInterface::UserInterface(Renderer& renderer)
	: m_Renderer(renderer) {
	m_InterfaceRenderData = DFNew InterfaceRenderData;
	m_InterfaceRenderer = DFNew InterfaceRenderer(*m_InterfaceRenderData);
}

df::UserInterface::~UserInterface() {
	DFDelete m_InterfaceRenderer;
	DFDelete m_InterfaceRenderData;
}

auto df::UserInterface::CreateFont(uint32 size, const df::Vector<uint8>& data)->Font* {
	Font* font = m_Fonts.Create(float(size));

	const int firstChar = 32;
	const int charCount = 96;

	const uint8* ttf_buffer = data.data();

	df::Vector<uint8> tmpBuffer1;
	df::Vector<uint8> tmpBuffer2;
	df::Vector<uint8> tmpBuffer3;

	tmpBuffer1.resize(1024 * 1024);
	tmpBuffer2.resize(1024 * 1024 * 4);
	tmpBuffer3.resize(sizeof(stbtt_bakedchar) * charCount);

	uint8* temp_bitmap = tmpBuffer1.data();
	uint8* temp_bitmap2 = tmpBuffer2.data();
	stbtt_bakedchar* cdata = (stbtt_bakedchar*)tmpBuffer3.data();

	int16 texWidth = 32;
	int16 texHeight = 32;

	int result = -1;
	while(result < 0) {
		texHeight *= 2;
		texWidth = texHeight;

		DFAssert(texHeight <= 1024 * 4, "Font texture size is too big!");

		result = stbtt_BakeFontBitmap(ttf_buffer, 0, float(size), temp_bitmap, texWidth, texHeight, firstChar, charCount, cdata);
	}

	for(int i = 0; i < texWidth * texHeight; ++i) {
		temp_bitmap2[i * 4 + 0] = 255;
		temp_bitmap2[i * 4 + 1] = 255;
		temp_bitmap2[i * 4 + 2] = 255;
		temp_bitmap2[i * 4 + 3] = temp_bitmap[i];
	}

	Texture* texture = m_Renderer.CreateTexture();
	texture->Create(texWidth, texHeight, ETextureFormat::R8G8B8A8_UNorm);
	texture->SetData(temp_bitmap2, texWidth * texHeight * 4);

	font->SetTexture(texture, texWidth, texHeight);

	Vector<Font::Glyph>& glyphs = font->AllocateGlyphs(firstChar, charCount);

	for(int i = 0; i < charCount; ++i) {
		const stbtt_bakedchar& bakedChar = cdata[i];
		Font::Glyph& glyph = glyphs[i];

		glyph.Width = uint8_t(bakedChar.x1 - bakedChar.x0);
		glyph.Height = uint8_t(bakedChar.y1 - bakedChar.y0);
		glyph.U0 = bakedChar.x0;
		glyph.V0 = bakedChar.y0;
		glyph.U1 = bakedChar.x1;
		glyph.V1 = bakedChar.y1;

		glyph.XOffset = int8_t(bakedChar.xoff);
		glyph.YOffset = int8_t(bakedChar.yoff);
		glyph.XAdvance = bakedChar.xadvance;
	}

	return font;
}

void df::UserInterface::DestroyFont(Font* font) {
	m_Renderer.DestroyTexture(font->GetTexture());
	m_Fonts.Destroy(font);
}

auto df::UserInterface::GetInterfaceRenderer()->InterfaceRenderer& {
	return *m_InterfaceRenderer;
}