#pragma once

#include "UINode.h"

#include <memory>
#include <tuple>
#include <vector>

#include "MapTile.h"

#define GRID (Grid::instance)

constexpr SDL_Color ZONE_COLORS[10] = {
	CGA_TRANSPARENT,	// 0
	CGA_LT_RED,			// 1
	CGA_BROWN,			// 2
	CGA_GREEN,			// 3
	CGA_LT_GREEN,		// 4
	CGA_CYAN,			// 5
	CGA_LT_CYAN,		// 6
	CGA_LT_BLUE,			// 7
	CGA_LT_MAGENTA,		// 8
	CGA_MAGENTA,		// 9
};

class Grid : public UINode
{
public:

	static std::unique_ptr<Grid> instance;
	static Grid* init();

	Grid();
	~Grid();

	virtual void draw(const SDL_Rect& container);
	void drawTiles(const SDL_Rect& container);
	void drawCursor(const SDL_Rect& container);
	void drawGrid(const SDL_Rect& container);
	void printGridNumb();
	void printLayerName();

	virtual bool onKeyDown(const SDL_Event& e);
	virtual bool onMouseDown(const SDL_Event& e);
	virtual bool onMouseUp(const SDL_Event& e);
	virtual bool onDrag(const SDL_Event& e);


	SDL_Point tileFromMouse(const SDL_Event& e);
	SDL_Point tileFromMouse();

	SDL_Point position;
	SDL_Point lastTilePlaced = { 0, 0 };

	SDL_Point editingTile = { -1, -1 };

	MapTile map[512][512];

	void changeLayer(int dir);
	void changeZone(int dir);

	int getCurrentLayer();
	int getCurrentZone();

	void save(std::wstring path);

	void setRotateVisible(bool rotate);
	void setRotation(int direction);

	int maxX = 10;
	int maxY = 10;
private:

	int floodToken = 0;

	int currentLayer = 1;
	int currentZone;

	void placeTile(int x, int y);
	void floodFill(int x, int y);
	bool floodable(int x, int y, int floodType, int floodToken);
	void removeTile(int x, int y, bool all);

	void placeEntity(int x, int y);
	void removeEntity(int x, int y);

	WallToken* mapWallTokens(_Out_ size_t* wallSize);
	EntityToken* mapEntityTokens(_Out_ size_t* entitySize);
	char* mapStringPool(_Out_ size_t* stringPoolSize);
	uint8_t* concatenateMap(
		_In_ WallToken* walls, size_t wallSize,
		_In_ EntityToken* entities, size_t entSize,
		_In_ char* stringPool, size_t stringPoolSize,
		_Out_ size_t* mapSize
	);

	bool writeSaveFile(std::wstring path, _In_ uint8_t* rawMap, size_t mapSize);

	//dynamic memory stack
	std::vector<bool> boolStack;
	std::vector<float> floatStack;
	std::vector<std::string> stringStack;

	bool rotationVisible;
	int direction;
	Texture* arrow;
};

