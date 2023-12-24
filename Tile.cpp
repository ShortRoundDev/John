#include "Tile.h"

#include "App.h"
#include "PropertiesEditor.h"
#include "Windows.h"

Tile::Tile(int t, int tileNum, Texture* image) : UINode({
		(uint16_t)((t % 3) * 72 + 8),
		(uint16_t)((t / 3) * 72 + 8),
		64, 64,
		"Resources/UI/Tile",
		CGA_TRANSPARENT,
		StyleDirection::TOP,
		StyleDirection::LEFT
	})
{
	this->tileNum = tileNum;
	if (!APP->tryLoadTexture("Resources/UI/TilePressed.png", "Resources/UI/TilePressed", &pressed))
	{
		MessageBoxA(NULL, "Failed to load tile image!", NULL, MB_OK);
	}
	depressed = this->image;
	this->tileImage = image;
}

void Tile::draw(const SDL_Rect& container)
{
	UINode::draw(container);
	auto offset = getDrawOffset(container);
	SDL_Rect dim = {
		offset.x + 2, offset.y + 2,
		calcX(style.width) - 4, calcY(style.height) - 4
	};

	SDL_RenderCopy(
		APP->renderer,
		tileImage->texture,
		&tileImage->src,
		&dim
	);
}

bool Tile::onMouseDown(const SDL_Event& e)
{	
	image = pressed;
	return true;
}

bool Tile::onMouseUp(const SDL_Event& e)
{
	image = depressed;

	if (e.button.button == SDL_BUTTON_LEFT)
	{
		APP->selectedTile = this->tileNum;
		APP->selectedTileTexture = this->tileImage;

		APP->selectedEntity = nullptr;
	}
	return true;
}