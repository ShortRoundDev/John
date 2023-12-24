#pragma once

#include "UINode.h"

class PropertyLine : public UINode
{
public:
	PropertyLine(uint16_t y, std::string name, UINode* button);
	~PropertyLine();
	std::string name;

	void draw(const SDL_Rect& container);
};

