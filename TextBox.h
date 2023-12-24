#pragma once

#include "UINode.h"

class TextBox : public UINode
{
public:
	TextBox(std::string* text);
	~TextBox();

	void draw(const SDL_Rect& container);

	bool onKeyDown(const SDL_Event& e);

	int cursorPos = 0;

	std::string* text;
};

