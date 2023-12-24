#pragma once

#include "UINode.h"

class TextButton : public UINode
{
public:
	TextButton(std::string * text);

	Texture* pressed;
	Texture* depressed;

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

	std::string* text;
};

