#pragma once

#include <DwarvenCore/Types.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/StringView.h>
#include <DwarvenCore/String.h>

namespace df {
	class Texture;
}

namespace df {
	class Font {
	public:
		struct Glyph {
			int8    XOffset;
			int8    YOffset;
			uint8   Width;
			uint8   Height;
			uint16  U0;
			uint16  V0;
			uint16  U1;
			uint16	V1;

			float   XAdvance;
		};

	public:
		Font(float height);

		void SetTexture(Texture* texture, uint16 textureWidth, uint16 textureHeight);
		auto AllocateGlyphs(int16 bias, uint16 count)->Vector<Glyph>&;

		auto GetHeight() const -> float;
		auto GetTexture() const->Texture*;
		auto GetInvTextureWidth() const -> float;
		auto GetInvTextureHeight() const -> float;

		auto operator [] (const char symbol) const -> const Glyph&;
		auto operator [] (const wchar_t symbol) const -> const Glyph&;

		auto CalculateTextWidth(const StringView text) const -> float;
		auto CalculateTextWidth(const WStringView text) const -> float;
		auto CalculateTextHeight() const  -> float;

	private:
		Texture* m_Texture = nullptr;
		int16 m_CharBias = 0;
		float m_InvTextureWidth = 1.0f;
		float m_InvTextureHeight = 1.0f;
		float m_Height = 1.0f;

		Vector<Glyph> m_Glyphs;
	};
}