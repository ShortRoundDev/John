#pragma once

#include "UINode.h"

class Grid;
class Texture;

class CompassButton : public UINode
{
public:
    CompassButton();

    void draw(const SDL_Rect& container);
    bool onMouseDown(const SDL_Event& e);

private:
    bool on;

    Texture* up;
    Texture* down;
};

