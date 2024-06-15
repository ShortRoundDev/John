#include "Grid.h"

#include <iostream>
#include <Windows.h>

#include "SDL2/SDL.h"

#include "App.h"
#include "LayerEditor.h"
#include "EntityButton.h"
#include "PropertiesEditor.h"

#include <nlohmann/json.hpp>

std::unique_ptr<Grid> Grid::instance = nullptr;

Grid* Grid::init()
{
	instance = std::make_unique<Grid>();
	return instance.get();
}

Grid::Grid() : UINode(
	{
		0, 34,
		UINode::PCT(100), UINode::PCT(100)
	}
)
{
	position = { 0, 0 };
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			this->map[j][i] = MapTile();
		}
	}
	addChild(new LayerEditor(this));
}

Grid::~Grid()
{

}

void Grid::draw(const SDL_Rect& container)
{
	drawTiles(container);
	drawCursor(container);
	drawGrid(container);

	UINode::draw(container); // draw children
	printGridNumb();
	printLayerName();
}

void Grid::drawTiles(const SDL_Rect& container)
{
	//if floor is active, draw transparent walls
	if (getCurrentLayer() == 0) {
		for (int y = position.y / 64; y < (position.y + APP->height) / 64 && y < 256; y++)
		{
			for (int x = position.x / 64; x < (position.x + APP->width) / 64 && x < 256; x++)
			{
				auto tile = this->map[x][y];

				int l2 = 1;
				int opacity = 128;

				int _x = (x * 64) - position.x;
				int _y = (y * 64) - position.y + 34;

				SDL_Rect dst = {
					_x, _y,
					64, 64
				};
				if (tile.texture[1] != nullptr)
				{
					auto tex = tile.texture[1]->texture;
					SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
					SDL_SetTextureAlphaMod(tex, opacity);
					SDL_RenderCopy(APP->renderer, tex, &tile.texture[1]->src, &dst);
					SDL_SetTextureAlphaMod(tex, 255);
				}
			}
		}
	}
	// draw tiles
	for (int y = position.y / 64; y < (position.y + APP->height) / 64 && y < 256; y++)
	{
		for (int x = position.x / 64; x < (position.x + APP->width) / 64 && x < 256; x++)
		{
			for (int l = 0; l <= getCurrentLayer(); l++)
			{
				auto tile = this->map[x][y];
				// l = 2 -> Ceiling
				// CurrentLayer = 3 -> Zones
				// Do not draw ceiling if zone layer is current
				if (!(l == 2 && getCurrentLayer() == 3) && l != 3)
				{
					int l2 = l;
					if (getCurrentLayer() == 3)
						l2++;
					int opacity = (int)(((float)(4 - (getCurrentLayer() - l2))) / 4.0f * 255);

					int _x = (x * 64) - position.x;
					int _y = (y * 64) - position.y + 34;

					SDL_Rect dst = {
						_x, _y,
						64, 64
					};
					if (tile.texture[l] != nullptr)
					{
						auto tex = tile.texture[l]->texture;
						SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
						SDL_SetTextureAlphaMod(tex, opacity);
						SDL_RenderCopy(APP->renderer, tex, &tile.texture[l]->src, &dst);
						SDL_SetTextureAlphaMod(tex, 255);
					}

					if (l == 1 && tile.entityTexture != nullptr)
					{
						SDL_SetTextureBlendMode(tile.entityTexture->texture, SDL_BLENDMODE_BLEND);
						SDL_SetTextureAlphaMod(tile.entityTexture->texture, opacity);
						SDL_RenderCopy(APP->renderer, tile.entityTexture->texture, &tile.entityTexture->src, &dst);
						SDL_SetTextureAlphaMod(tile.entityTexture->texture, 255);
					}
				}
				// Draw zone selectors
				else if (l == 3 && tile.token.zone > 0)
				{

					int _x = (x * 64) - position.x;
					int _y = (y * 64) - position.y + 34;

					SDL_Rect dst = {
						_x + 4, _y + 4,
						64 - 8, 64 - 8
					};
					auto col = ZONE_COLORS[tile.token.zone];
					SDL_SetRenderDrawColor(APP->renderer, col.r, col.g, col.b, col.a);
					SDL_RenderFillRect(APP->renderer, &dst);

					SDL_Rect src = {
						tile.token.zone * 14, 0,
						14, 18
					};

					dst = {
						_x + 10, _y + 10,
						14, 18
					};

					SDL_RenderCopy(APP->renderer, APP->numbers->texture, &src, &dst);

				}
			}
		}
	}
}

void Grid::drawCursor(const SDL_Rect& container)
{
	// draw cursor
	if (getCurrentLayer() != 3)
	{
		int x, y;

		SDL_GetMouseState(&x, &y);
		x = ((x + position.x) / 64 * 64) - position.x;
		y = (y - 34 + position.y) / 64 * 64 - position.y + 34;

		SDL_Rect dst = {
			x, y,
			64, 64
		};
		Texture* t = nullptr;

		if (APP->selectedTileTexture != nullptr)
		{
			t = APP->selectedTileTexture;
		}
		else if (APP->selectedEntity != nullptr)
		{
			t = APP->selectedEntity->entTexture;
		}
		if (t != nullptr)
		{
			SDL_SetTextureAlphaMod(t->texture, 128);

			SDL_RenderCopy(
				APP->renderer,
				t->texture,
				&t->src,
				&dst
			);

			SDL_SetTextureAlphaMod(t->texture, 255);
		}

	}
	else if (getCurrentLayer() == 3 && getCurrentZone() > 0)
	{
		int x, y;

		SDL_GetMouseState(&x, &y);
		x = ((x + position.x) / 64 * 64) - position.x;
		y = (y - 34 + position.y) / 64 * 64 - position.y + 34;

		SDL_Rect dst = {
			x + 4, y + 4,
			64 - 8, 64 - 8
		};

		SDL_Color col = ZONE_COLORS[getCurrentZone()];
		SDL_SetRenderDrawBlendMode(APP->renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(APP->renderer, col.r, col.g, col.b, 128);
		SDL_RenderFillRect(APP->renderer, &dst);
		SDL_SetRenderDrawBlendMode(APP->renderer, SDL_BLENDMODE_NONE);
	}
}

void Grid::drawGrid(const SDL_Rect& container)
{
	SDL_Rect dimensions = {
		0, 0,
		calcX(style.width), calcY(style.height)
	};

	// draw grid
	SDL_SetRenderDrawColor(APP->renderer, CGA_LT_GRAY.r, CGA_LT_GRAY.g, CGA_LT_GRAY.b, CGA_LT_GRAY.a);
	for (int x = (-position.x % 64); x < dimensions.w + 64; x += 64)
	{
		for (int y = style.y + (-position.y % 64); y < dimensions.h; y += 64)
		{
			SDL_Rect dot = {
				x - 2, y - 2, 4, 4
			};
			SDL_RenderFillRect(APP->renderer, &dot);
		}
	}

	SDL_SetRenderDrawColor(APP->renderer, CGA_YELLOW.r, CGA_YELLOW.g, CGA_YELLOW.b, CGA_YELLOW.a);
	//draw boundaries
	if (position.x < maxX * 64 && position.y < maxY * 64)
	{
		for (int y = 0; y < maxY * 4 + 1; y++) // * 4 for 16px grid
		{
			SDL_Rect dot = {
				maxX * 64 - position.x - 2, y * 16 - position.y - 2 + 34,
				4, 4
			};
			SDL_RenderFillRect(APP->renderer, &dot);
		}

		for (int x = 0; x < maxX * 4; x++) // * 4 for 16px grid
		{
			SDL_Rect dot = {
				x * 16 - position.x - 2, maxY * 64 - position.y - 2 + 34,
				4, 4
			};
			SDL_RenderFillRect(APP->renderer, &dot);
		}
	}
}

void Grid::printGridNumb()
{
	auto pos = tileFromMouse();
	APP->drawText(0, APP->height - 20, (std::to_string(pos.x) + ", " + std::to_string(pos.y)).c_str());
}

void Grid::printLayerName()
{
	std::string layerName = "";
	switch (getCurrentLayer()) {
	case 0:
		layerName = "Floor";
		break;
	case 1:
		layerName = "Walls";
		break;
	case 2:
		layerName = "Ceiling";
		break;
	case 3:
		layerName = "Zones";
		break;
	}
	APP->drawText(0, 48, layerName.c_str());
}



bool Grid::onKeyDown(const SDL_Event& e)
{
	int diff = 20;
	if (e.key.keysym.mod & KMOD_LSHIFT)
	{
		diff = 64;
	}
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_LEFT:
	{
		position.x -= diff;
		if (position.x < 0)
			position.x = 0;
		break;
	}
	case SDL_SCANCODE_RIGHT:
	{
		position.x += diff;
		break;
	}
	case SDL_SCANCODE_UP:
	{
		position.y -= diff;
		if (position.y < 0)
			position.y = 0;
		break;
	}
	case SDL_SCANCODE_DOWN:
	{
		position.y += diff;
	}
	}
	return true;
}

bool Grid::onMouseDown(const SDL_Event& e)
{
	SDL_Point mouse = {
		e.button.x,
		e.button.y
	};
	SDL_Rect actualSize = {
		0, 34,
		calcX(PCT(100)) - 256, calcY(PCT(100)) - 34
	};
	if (!SDL_PointInRect(&mouse, &actualSize))
	{
		return true;
	}

	SDL_Point tilePos = tileFromMouse(e);
	//MessageBoxA(NULL, (std::to_string(mouse.x) + ", " + std::to_string(mouse.y)).c_str(), NULL, MB_OK);


	if (APP->mouseState & MOUSE_LEFT)
	{
		if (APP->selectedTile != 0)
		{
			if (APP->keymap[SDL_SCANCODE_LSHIFT] == true)
			{
				floodFill(tilePos.x, tilePos.y);
			}
			else
			{
				placeTile(tilePos.x, tilePos.y);
			}
		}
		else if(APP->selectedEntity != nullptr)
		{
			placeEntity(tilePos.x, tilePos.y);
		}
	}
	else if (APP->mouseState & MOUSE_MIDDLE)
	{
		if (APP->keymap[SDL_SCANCODE_LCTRL])
			removeEntity(tilePos.x, tilePos.y);
		else
			removeTile(tilePos.x, tilePos.y, false);
	}
	return true;
}

bool getBoolOr(nlohmann::json const& json, std::string const& key, bool def)
{
	try
	{
        return json.at(key).get<bool>();
    }
	catch (nlohmann::json::out_of_range)
	{
        return def;
    }
	catch (nlohmann::json::type_error)
	{
        return def;
    }
	catch (nlohmann::json::exception)
	{
        return def;
    }
}

float getFloatOr(nlohmann::json const& json, std::string const& key, bool def)
{
	try
	{
        return json.at(key).get<float>();
    }
	catch (nlohmann::json::out_of_range)
	{
        return def;
    }
	catch (nlohmann::json::type_error)
	{
        return def;
    }
	catch (nlohmann::json::exception)
	{
        return def;
    }

}

std::string getStringOr(nlohmann::json const& json, std::string const& key, std::string def)
{
	try
	{
        return json.at(key).get<std::string>();
    }
	catch (nlohmann::json::out_of_range)
	{
        return def;
    }
	catch (nlohmann::json::type_error)
	{
        return def;
    }
	catch (nlohmann::json::exception)
	{
        return def;
    }

}

bool Grid::onMouseUp(const SDL_Event& e)
{
	if (e.button.button == SDL_BUTTON_RIGHT)
	{
		SDL_Point tilePos = tileFromMouse(e);
		MapTile* tile = &(map[tilePos.x][tilePos.y]);
		if(APP->keymap[SDL_SCANCODE_LCTRL])
		{		// edit entity
			if (tile->entityToken == nullptr)
			{
				return true;
			}
			auto metadata = APP->entityMetadata[tile->entityToken->entityId];
			PROPS->clear();
			boolStack.clear();
			floatStack.clear();
			stringStack.clear();

			for (auto field : metadata)
			{

				if (field.type == FieldType::JOHN_BOOL)
				{
					bool val = getBoolOr(tile->entityJsonConfig, field.name, false);
					PROPS->addBoolean(field.name, val, [tile, field](bool value)
						{
							tile->entityJsonConfig[field.name] = value;
						});
                }
				else if (field.type == FieldType::JOHN_FLOAT)
				{
					float val = getFloatOr(tile->entityJsonConfig, field.name, false);
					PROPS->addFloat(field.name, val, [tile, field](float value)
						{
							tile->entityJsonConfig[field.name] = value;
						});
                }
				else if (field.type == FieldType::JOHN_STRING)
				{
					std::string val = getStringOr(tile->entityJsonConfig, field.name, "");
					PROPS->addText(field.name, val, [tile, field](std::string value)
						{
							tile->entityJsonConfig[field.name] = value;
						});
                }
			}
		}
		else	// edit tile
		{
			if (tile->token.wallType == 0)
				return true;
			PROPS->clear();
			PROPS->addBoolean("isDoor", &(tile->isDoor));
			PROPS->addNumber("lockType", &(tile->token.lockType));
			PROPS->addText("config", &(tile->tileConfig));
		}
	}
	return true;
}



bool Grid::onDrag(const SDL_Event& e)
{
	static int dragged = 0;
	SDL_Point tilePos = tileFromMouse(e);
	if ((lastTilePlaced.x == tilePos.x) && (lastTilePlaced.y == tilePos.y))
	{
		return true;
	}

	if (APP->mouseState & MOUSE_LEFT)
		placeTile(tilePos.x, tilePos.y);
	else if (APP->mouseState & MOUSE_MIDDLE)
		removeTile(tilePos.x, tilePos.y, false);
	return true;
}

SDL_Point Grid::tileFromMouse(const SDL_Event& e)
{
	SDL_Point mouse = {
		e.button.x,
		e.button.y
	};

	mouse.x = ((mouse.x + position.x) / 64);
	mouse.y = (mouse.y - 34 + position.y) / 64;

	return mouse;
}

SDL_Point Grid::tileFromMouse()
{
	int x, y;

	SDL_GetMouseState(&x, &y);
	
	SDL_Point mouse = {
		x,
		y
	};

	mouse.x = ((mouse.x + position.x) / 64);
	mouse.y = ((mouse.y - 34 + position.y) / 64);

	return mouse;
}

void Grid::changeLayer(int dir)
{
	this->currentLayer += dir;
	if (this->currentLayer < 0)
		this->currentLayer = 0;
	else if (this->currentLayer > 3)
		this->currentLayer = 3;
}

void Grid::changeZone(int dir)
{
	this->currentZone += dir;
	if (this->currentZone < 0)
		this->currentZone = 0;
	else if (this->currentZone > 9)
		this->currentZone = 9;
}

int Grid::getCurrentLayer()
{
	return currentLayer;
}

int Grid::getCurrentZone()
{
	return currentZone;
}

void Grid::placeTile(int x, int y)
{
	if (x < 0 || x >= 512)
		return;
	if (y < 0 || y >= 512)
		return;
	if (x >= maxX)
		maxX = x + 1;
	if (y >= maxY)
		maxY = y + 1;
	if (getCurrentLayer() != LAYER_GRID)
	{
		map[x][y].set(getCurrentLayer(), APP->selectedTile, APP->selectedTileTexture);
	}
	else
	{
		map[x][y].set(getCurrentLayer(), getCurrentZone(), nullptr);
	}
	lastTilePlaced = { x, y };
}

void Grid::floodFill(int x, int y)
{
	if (x < 0 || x >= 512 || y < 0 || y >= 512 || x > maxX || y > maxY)
		return;
	int floodType = -1;
	switch (getCurrentLayer())
	{
	case LAYER_FLOOR:
		floodType = map[x][y].token.floor;
		break;
	case LAYER_WALLS:
		floodType = map[x][y].token.wallType;
		break;
	case LAYER_CEIL:
		floodType = map[x][y].token.ceiling;
		break;
	case LAYER_GRID:
		floodType = map[x][y].token.zone;
		break;
	}

	floodToken++;

	std::vector<std::tuple<int, int>> history = std::vector<std::tuple<int, int>>();
	std::map<std::tuple<int, int>, bool> permanentHistory = std::map<std::tuple<int, int>, bool>();
	history.push_back(std::tuple<int, int>(x, y));
	while (!history.empty())
	{
		std::tuple<int, int> top = history.back();
		history.pop_back();
		int x = std::get<0>(top),
			y = std::get<1>(top);
		if (floodable(x, y, floodType, floodToken))
		{
			placeTile(x, y);
			map[x][y].floodToken = floodToken;
			if (floodable(x - 1, y, floodType, floodToken))
				history.push_back(std::tuple<int, int>(x - 1, y));
			if (floodable(x + 1, y, floodType, floodToken))
				history.push_back(std::tuple<int, int>(x + 1, y));
			if (floodable(x, y - 1, floodType, floodToken))
				history.push_back(std::tuple<int, int>(x, y - 1));
			if (floodable(x, y + 1, floodType, floodToken))
				history.push_back(std::tuple<int, int>(x, y + 1));
		}
	}
}

bool Grid::floodable(int x, int y, int floodType, int floodToken)
{
	if (x < 0 || x >= 512 || y < 0 || y >= 512 || x > maxX || y > maxY)
		return false;

	if (map[x][y].floodToken == floodToken)
		return false;

	switch (getCurrentLayer())
	{
	case LAYER_FLOOR:
		return map[x][y].token.floor == floodType;
	case LAYER_WALLS:
		return map[x][y].token.wallType == floodType;
	case LAYER_CEIL:
		return map[x][y].token.ceiling == floodType;
	case LAYER_GRID:
		return map[x][y].token.zone == floodType;
	}
	return false;
}

void Grid::removeTile(int x, int y, bool all)
{
	if (x < 0 || x >= 512)
		return;
	if (y < 0 || y >= 512)
		return;

	if (all)
	{
		for (int i = 0; i < 3; i++)
		{
			map[x][y].set(i, 0, nullptr);
		}
	}
	else
	{
		map[x][y].set(getCurrentLayer(), 0, nullptr);
	}

	int _maxX = 0, _maxY = 0;
	for (int i = 0; i < maxX; i++) {
		// check far right line
		for (int j = 0; j < maxY; j++) {
			auto tile = &(map[i][j]);
			if (!(tile->token.ceiling == 0 && tile->token.floor == 0 && tile->token.wallType == 0 && tile->entityToken == nullptr)) {
				if (i > _maxX) {
					_maxX = i;
				}
				if (j > _maxY) {
					_maxY = j;
				}
			}
		}
	}
	_maxX += 1;
	_maxY += 1;

	if (_maxX < 10) {
		_maxX = 10;
	}

	if (_maxY < 10) {
		_maxY = 10;
	}

	if (_maxX < maxX) {
		maxX = _maxX;
	}
	if (_maxY < maxY) {
		maxY = _maxY;
	}

	lastTilePlaced = { x, y };
}

void Grid::placeEntity(int x, int y)
{
	if (getCurrentLayer() != 1)
		return;
	if (x < 0 || x >= 512 || y < 0 || y >= 512)
		return;
	if (map[x][y].entityToken == nullptr)
		map[x][y].entityToken = new EntityToken;
	map[x][y].entityToken->entityId = APP->selectedEntity->entId;
	map[x][y].entityToken->x = x;
	map[x][y].entityToken->y = y;
	map[x][y].entityTexture = APP->selectedEntity->entTexture;
	if (x >= maxX) {
		maxX = x + 1;
	}
	if (y >= maxY) {
		maxY = y + 1;
	}
}

void Grid::removeEntity(int x, int y)
{
	if (getCurrentLayer() != 1)
		return;
	if (x < 0 || x >= 512 || y < 0 || y >= 512)
		return;
	if (map[x][y].entityToken == nullptr)
		return;
#pragma message ("WARNING: Potential memory leak in RemoveEntity. Need to distinguish between a file-loaded entity and a malloc'd entity!!!")
	//delete map[x][y].entityToken;		// 
	map[x][y].entityToken = nullptr;
	map[x][y].entityConfig = "";
	map[x][y].entityTexture = nullptr;	// lose the reference, but the texture should probably still live
}

void Grid::save(std::wstring path)
{
	size_t	wallSize		= 0,
			entitySize		= 0,
			stringPoolSize	= 0,
			mapSize			= 0;

	char*			stringPool	= mapStringPool(&stringPoolSize);
	WallToken*		walls		= mapWallTokens(&wallSize);
	EntityToken*	entities	= mapEntityTokens(&entitySize);

	uint8_t* rawMap = concatenateMap(
		walls,		wallSize,
		entities,	entitySize,
		stringPool, stringPoolSize,
		
		&mapSize
	);

	walls		= nullptr;
	entities	= nullptr;
	stringPool	= nullptr;

	writeSaveFile(path, rawMap, mapSize);
	rawMap = nullptr;
}

WallToken* Grid::mapWallTokens(_Out_ size_t* wallSize)
{
	int idx = 0;
	*wallSize = maxX * maxY * sizeof(WallToken);
	WallToken* walls = (WallToken*)calloc(maxX * maxY, sizeof(WallToken));
	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			// manually convert some fields:
			map[x][y].token.isDoor = map[x][y].isDoor ? 1 : 0;

			// copy the token
			memcpy(walls + idx, &(map[x][y].token), sizeof(WallToken));
			idx++;
		}
	}
	return walls;
}

EntityToken* Grid::mapEntityTokens(_Out_ size_t* entitySize)
{
	std::vector<EntityToken*> entVec = std::vector<EntityToken*>();
	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			if (map[x][y].entityToken != nullptr)
			{
				map[x][y].entityToken->x = x;
				map[x][y].entityToken->y = y;
				entVec.push_back(map[x][y].entityToken);
			}
		}
	}
	EntityToken* entities = nullptr;
	*entitySize = entVec.size() * sizeof(EntityToken);

	if (entVec.size() > 0)
	{
		entities = (EntityToken*)calloc(entVec.size(), sizeof(EntityToken));
		int idx = 0;
		for (EntityToken* entTok : entVec)
		{
			memcpy(entities + idx, entTok, sizeof(EntityToken));
			idx++;
		}
	}
	return entities;
}

char* Grid::mapStringPool(_Out_ size_t* stringPoolSize)
{
	*stringPoolSize = 0;
	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			std::stringstream ss;
			/*ss << map[x][y].tileJsonConfig;
			std::string tileConfig = ss.str();
			if (!tileConfig.empty())
			{
				(*stringPoolSize) += tileConfig.length() + 1;
			}*/

			ss.clear();

			if (!map[x][y].entityJsonConfig.is_null())
			{
				ss << map[x][y].entityJsonConfig;
				std::string entConfig = ss.str();
				map[x][y].entityConfig = entConfig;
				if (!entConfig.empty())
				{
					(*stringPoolSize) += entConfig.length() + 1;
				}
			}
		}
	}
	*stringPoolSize += 1;

	char* stringPool = (char*) calloc(*stringPoolSize + 2, sizeof(char));
	uint64_t poolCursor = 1; // 0 initialize to null

	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			std::string config = map[x][y].tileConfig;
			if (!config.empty())
			{
				const char* c = config.c_str();
				memcpy(stringPool + poolCursor, c, config.length());
				map[x][y].token.message = (char*)poolCursor; // these will have to be remapped later
				poolCursor += config.length() + 1;
			}
			else {
				map[x][y].token.message = NULL;
			}

			std::string entConfig = map[x][y].entityConfig;
			if (!entConfig.empty())
			{
				memcpy(stringPool + poolCursor, entConfig.c_str(), entConfig.length());
				map[x][y].entityToken->config = (char*)poolCursor; // these will have to be remapped later
				poolCursor += entConfig.length() + 1;
			}
			else if(map[x][y].entityToken != NULL) {
				map[x][y].entityToken->config = NULL;
			}
		}
	}

	return stringPool;
}

uint8_t* Grid::concatenateMap(
	_In_ WallToken* walls,		size_t wallSize,
	_In_ EntityToken* entities,	size_t entSize,
	_In_ char* stringPool,		size_t stringPoolSize,
	
	_Out_ size_t* mapSize
)
{
	uint8_t* rawMap = (uint8_t*)calloc(sizeof(LevelToken) + wallSize + entSize + stringPoolSize + 1, sizeof(uint8_t));

	uint8_t* mapCursor = rawMap;

	LevelToken* header = (LevelToken*)mapCursor;
	
	*header = {
		{'H', 'A', 'M'}, // Watermark
		2,	//version #
		(uint16_t)maxX,	// width
		(uint16_t)maxY,	// height
		entSize/sizeof(EntityToken), // total Entities
		(WallToken*)(sizeof(LevelToken)), // wall location start
		(EntityToken*)(sizeof(LevelToken) + wallSize) // entity location start
	};

	mapCursor += sizeof(LevelToken);
	uint64_t stringOffLocal = (uint64_t)(sizeof(LevelToken) + wallSize + entSize);
	// copy
	WallToken* wallPointer = (WallToken*)mapCursor;
	memcpy(mapCursor, walls, wallSize);
	for (WallToken* wall = (WallToken*)mapCursor; wall < (WallToken*)(mapCursor + wallSize); wall++)
	{
		if (wall->message != NULL)
		{
			wall->message += stringOffLocal;
		}
	}

	mapCursor += wallSize;
	// These can be null. Walls can never be null	

	EntityToken* entityPointer = (EntityToken*) mapCursor;
	if (entSize != 0)
	{
		memcpy(mapCursor, entities, entSize);

		for (EntityToken* ent = (EntityToken*)mapCursor; ent < (EntityToken*)(mapCursor + entSize); ent++)
		{
			if (ent->config != NULL)
			{
				ent->config += stringOffLocal;
			}
		}

		mapCursor += entSize;
	}

	if (stringPoolSize != 0)
	{
		memcpy(mapCursor, stringPool, stringPoolSize);
		mapCursor += stringPoolSize;
	}
	
	// free
	free(walls);
	// These can be null. Walls can never be null
	if(entities != nullptr)
		free(entities);
	if(stringPool != nullptr)
		free(stringPool);

	*mapSize = sizeof(LevelToken) + wallSize + entSize + stringPoolSize;

	return rawMap;
}

bool Grid::writeSaveFile(std::wstring path, _In_ uint8_t* rawMap, size_t mapSize)
{
	FILE* wfp = _wfopen(path.c_str(), L"wb");
	if (!wfp)
	{
		MessageBoxW(NULL, (L"Failed to open file [" + path + L"] for saving!").c_str(), NULL, MB_OK);
		free(rawMap);
		return false;
	}

	fwrite(rawMap, sizeof(uint8_t), mapSize, wfp);
	fclose(wfp);

	free(rawMap);
	return true;
}