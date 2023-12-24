#pragma once
#include <string>
#include "MapTile.h"
class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	bool loadMap(std::wstring path);

	LevelToken* level;
private:
	bool fixEntities(uint8_t* data);
	bool fixWalls(uint8_t* data);
	bool populateMap();
};

