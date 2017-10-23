#pragma once

struct TextCache
{
public:
	TextCache(const std::string& text, vec2 pos, vec4 col) :
		Text(text), Position(pos), Color(col) {}

	const std::string Text;
	vec2 Position;
	vec4 Color;

private:
	TextCache& operator=(const TextCache &tmp);
};

struct FontMetric
{
	//BMFONT
	bool IsValid;
	wchar_t Character;
	uint16 Width;
	uint16 Height;
	int16 OffsetX;
	int16 OffsetY;
	int16 AdvanceX;
	uint8 Page;
	uint8 Channel;
	vec2 TexCoord;
};

class TextureData;
class TextRenderer;

class SpriteFont
{
public:
	SpriteFont();
	virtual ~SpriteFont();

	static bool IsCharValid(const wchar_t& character);

	static const int32 MAX_CHAR_ID = 255;
	static const int32 MIN_CHAR_ID = 0;
	static const int32 CHAR_COUNT = MAX_CHAR_ID - MIN_CHAR_ID + 1;

private:
	friend class TextRenderer;
	friend class FontLoader;

	FontMetric& GetMetric(const wchar_t& character) { return m_CharTable[character - MIN_CHAR_ID]; };

	FontMetric m_CharTable[CHAR_COUNT];
	std::vector<TextCache> m_TextCache;

	int16 m_FontSize;
	std::string m_FontName;
	int32 m_CharacterCount;
	int32 m_CharacterSpacing;
	int32 m_TextureWidth;
	int32 m_TextureHeight;
	int32 m_BufferStart, m_BufferSize;
	TextureData *m_pTexture;
	bool m_IsAddedToRenderer;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteFont(const SpriteFont& t);
	SpriteFont& operator=(const SpriteFont& t);
};

