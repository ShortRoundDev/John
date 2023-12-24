#pragma once
#include "UIButton.h"

class SaveButton : public UIButton
{
public:
	SaveButton();
	
	bool onMouseUp(const SDL_Event& e);
};

