#include "MapTile.h"

MapTile::MapTile()
{
	token.wallType = 0;
	token.isDoor = 0;
	token.lockType = 0;
	token.floor = 0;
	token.ceiling = 0;
	token.zone = 0;
	token.message = NULL;
	// to do: automatically make something a door/locked

	texture[LAYER_FLOOR] = nullptr;
	texture[LAYER_WALLS] = nullptr;
	texture[LAYER_CEIL] = nullptr;
	entityToken = nullptr;
	entityTexture = nullptr;
}

MapTile::~MapTile()
{

}

void MapTile::set(int layer, int tileNum, Texture* tileTexture)
{
	switch (layer)
	{
	case LAYER_FLOOR:
	{
		token.floor = tileNum;
		break;
	}
	case LAYER_WALLS:
	{
		token.wallType = tileNum;
		break;
	}
	case LAYER_CEIL :
	{
		token.ceiling = tileNum;
		break;
	}
	default:
	{
		token.zone = tileNum; // "tilenum" here is the grid #
		return;
	}
	}

	texture[layer] = tileTexture;
}