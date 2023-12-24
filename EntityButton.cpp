#include "EntityButton.h"

#include "App.h"
#include "Windows.h"

EntityButton::EntityButton
(
	int tileOrder,
	int entId,
	Texture* entTexture,
	std::string entName
) : UINode(
	{
		(uint16_t)((tileOrder % 3) * 72 + 8),
		(uint16_t)((tileOrder / 3) * 72 + 8),
		64, 64,
		"Resources/UI/Tile"
	}
	)
{
	this->entId = entId;
	this->entTexture = entTexture;
	this->entName = entName;

	depressed = image;

	if (!APP->tryLoadTexture("Resources/UI/TilePressed.png", "Resources/UI/TilePressed", &pressed))
	{
		MessageBoxA(NULL, "Failed to load tile image!", NULL, MB_OK);
	}

}

void EntityButton::draw(const SDL_Rect& container)
{
	UINode::draw(container);
	auto offset = getDrawOffset(container);
	SDL_Rect dim = {
		offset.x + 2, offset.y + 2,
		calcX(style.width) - 4, calcY(style.height) - 4
	};

	SDL_RenderCopy(
		APP->renderer,
		entTexture->texture,
		&entTexture->src,
		&dim
	);
}

bool EntityButton::onMouseDown(const SDL_Event& e)
{
	image = pressed;
	return true;
}

bool EntityButton::onMouseUp(const SDL_Event& e)
{
	image = depressed;

	APP->selectedTile = 0;
	APP->selectedTileTexture = nullptr;

	APP->selectedEntity = this;
	return true;
}