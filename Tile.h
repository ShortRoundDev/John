#pragma once

#include "UINode.h"

class Tile : public UINode
{
public:
	Tile(int t, int tileNum, Texture* image);

	Texture* pressed;
	Texture* depressed;
	Texture* tileImage;
	int tileNum;

	virtual void draw(const SDL_Rect& container);
	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);
};

