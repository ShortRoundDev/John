#pragma once
#include "UINode.h"

class NumberInput : public UINode
{
public:
	NumberInput(uint8_t* number);
	~NumberInput();

	uint8_t* number;

	void draw(const SDL_Rect& container);
};

