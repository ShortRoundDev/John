#pragma once
#include "UINode.h"

#include <string>

class UIButton : public UINode
{
public:
	UIButton(Style style);
	~UIButton();

	Texture* pressed;
	Texture* depressed;

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);
};

