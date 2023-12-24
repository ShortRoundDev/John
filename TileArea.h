#pragma once

#include <map>

#include "UINode.h"
#include "Texture.h"

class TileArea : public UINode
{
public:
	TileArea();
	~TileArea();

	std::map<uint16_t, Texture*> tileTextures;
private:
	void loadTiles();
};

