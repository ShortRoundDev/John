#pragma once

#include "UINode.h"

class Checkbox : public UINode
{
public:
	Checkbox(bool* toCheck);

	Texture* checkDepressed;
	Texture* checkPressed;

	Texture* uncheckDepressed;
	Texture* uncheckPressed;

	bool* toCheck;

	bool onMouseUp(const SDL_Event& e);
	bool onMouseDown(const SDL_Event& e);
};

