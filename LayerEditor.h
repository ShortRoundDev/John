#pragma once

#include "UINode.h"

class Grid;

class LayerEditor : public UINode
{
public:
	LayerEditor(Grid* grid);
	~LayerEditor();

	void draw(const SDL_Rect& container);

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

	Grid* grid;
};

