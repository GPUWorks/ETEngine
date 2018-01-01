#pragma once
#include "../staticDependancies/glad/glad.h"
#include "ContentLoader.hpp"
#include "../Graphics/TextureData.hpp"
#include <string>

class TextureLoader : public ContentLoader<TextureData>
{
public:
	TextureLoader();
	~TextureLoader();

	void UseSrgb(bool use) { m_UseSrgb = use; }
	void ForceResolution(bool force) { m_ForceRes = force; }

protected:
	virtual TextureData* LoadContent(const std::string& assetFile);
	virtual void Destroy(TextureData* objToDestroy);

	bool m_UseSrgb = false;
	bool m_ForceRes = false;
};

