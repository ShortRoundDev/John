#pragma once

#include "UIButton.h"

class LoadButton : public UIButton
{
public:
	LoadButton();

	bool onMouseUp(const SDL_Event& e);
};

