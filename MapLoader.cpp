#include "MapLoader.h"
#include <Windows.h>
#include "FileHandling.h"

#include "App.h"
#include "Grid.h"

#include <nlohmann/json.hpp>

MapLoader::MapLoader()
{

}

MapLoader::~MapLoader()
{

}

bool MapLoader::loadMap(std::wstring path)
{
	uint8_t* data = NULL;
	DWORD size;
	openBinaryFile(path, &data, &size);

	level = (LevelToken*)data;
	level->walls = (WallToken*)(data + (uint64_t)level->walls);
	level->entities = (EntityToken*)(data + (uint64_t)level->entities);

	fixEntities(data);
	fixWalls(data);

	populateMap();

	return true;
}

bool MapLoader::fixEntities(uint8_t* data)
{
	for (int i = 0; i < level->totalEntities; i++) {
		if (level->entities[i].config != NULL) {
			level->entities[i].config = (char*)(data + (uint64_t)(level->entities[i].config));
		}
	}
	return true;
}

bool MapLoader::fixWalls(uint8_t* data)
{
	for (int i = 0; i < level->height; i++) {
		for (int j = 0; j < level->width; j++) {
			int idx = (i * level->width) + j;
			if (level->walls[idx].message != NULL) {
				level->walls[idx].message = (char*)(data + (uint64_t)(level->walls[idx].message));
			}
		}
	}
	return true;
}

bool MapLoader::populateMap()
{
	for (int i = 0; i < level->totalEntities; i++) {
		auto entity = &(level->entities[i]);
		auto tile = &(GRID->map[entity->x][entity->y]);
		tile->entityToken = entity;
		if (entity->config != NULL) {
			tile->entityConfig = std::string(entity->config);
			try
			{
                tile->entityJsonConfig = nlohmann::json::parse(entity->config);
            }
			catch (nlohmann::json::parse_error& e)
			{
				tile->entityJsonConfig = nlohmann::json();
			}
		}
		tile->entityTexture = APP->textures[APP->idTextureMapping[entity->entityId]];
		
	}

	for (int i = 0; i < level->height; i++) {
		for (int j = 0; j < level->width; j++) {
			int idx = (i * level->width) + j;
			auto tile = &(GRID->map[j][i]);
			tile->token = level->walls[idx];
			if (tile->token.message != NULL) {
				tile->tileConfig = std::string(tile->token.message);
				try
				{
                    tile->tileJsonConfig = nlohmann::json::parse(tile->token.message);
                }
				catch (nlohmann::json::parse_error& e)
				{
                    tile->tileJsonConfig = nlohmann::json();
                }
			}
			tile->isDoor = tile->token.isDoor != 0;
			tile->texture[0] = APP->textures[APP->idTextureMapping[tile->token.floor]];
			tile->texture[1] = APP->textures[APP->idTextureMapping[tile->token.wallType]];
			tile->texture[2] = APP->textures[APP->idTextureMapping[tile->token.ceiling]];
		}
	}

	GRID->maxX = level->width;
	GRID->maxY = level->height;

	return true;
}