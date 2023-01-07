#include <DwarfUserInterface/Font.h>

df::Font::Font(float height)
	: m_Height(height) {}

void df::Font::SetTexture(Texture* texture, uint16 textureWidth, uint16 textureHeight) {
	m_Texture = texture;
	m_InvTextureWidth = 1.0f / float(textureWidth);
	m_InvTextureHeight = 1.0f / float(textureHeight);
}

auto df::Font::AllocateGlyphs(int16 bias, uint16 count) -> df::Vector<Glyph>& {
	m_Glyphs.resize(count);
	m_CharBias = bias;

	return m_Glyphs;
}

auto df::Font::GetHeight() const -> float {
	return m_Height;
}

auto df::Font::GetTexture() const -> Texture* {
	return m_Texture;
}

auto df::Font::GetInvTextureWidth() const -> float {
	return m_InvTextureWidth;
}

auto df::Font::GetInvTextureHeight() const -> float {
	return m_InvTextureHeight;
}

auto df::Font::operator[](const char symbol) const -> const Glyph& {
	return m_Glyphs[int32(symbol) - m_CharBias];
}

auto df::Font::operator[](const wchar_t symbol) const -> const Glyph& {
	return m_Glyphs[int32(symbol) - int32(m_CharBias)];
}

auto df::Font::CalculateTextWidth(const df::StringView text) const -> float {
	float result = 0.0f;
	const size_t textLength = text.length();
	for(size_t i = 0; i < textLength; ++i) {
		const int32 idx = int32(text[i]) - m_CharBias;
		result += m_Glyphs[idx].XAdvance;
	}
	return result;
}

auto df::Font::CalculateTextWidth(const df::WStringView text) const -> float {
	float result = 0.0f;
	const size_t textLength = text.length();
	for(size_t i = 0; i < textLength; ++i) {
		const int32 idx = int32(text[i]) - m_CharBias;
		result += m_Glyphs[idx].XAdvance;
	}
	return result;
}

auto df::Font::CalculateTextHeight() const -> float {
	return m_Height;
}