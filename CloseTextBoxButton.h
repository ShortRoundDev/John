#pragma once

#include "UINode.h"

class CloseTextBoxButton : public UINode
{
public:
	CloseTextBoxButton(UINode* parent);
	~CloseTextBoxButton();

	Texture* depressed;
	Texture* pressed;

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

private:
	UINode* parent;
};