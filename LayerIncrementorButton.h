#pragma once
#include "UINode.h"
#include <functional>

class LayerIncrementorButton : public UINode
{
public:
	LayerIncrementorButton(uint16_t x, uint16_t y, bool up, std::function<void()> callback);
	
	std::function<void()> increment;

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

	Texture* pressed;
	Texture* depressed;
};

