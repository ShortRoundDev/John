#pragma once
#include "UINode.h"

enum class Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class ArrowButton : public UINode
{
public:
    ArrowButton();

    bool onMouseDown(const SDL_Event& e);
private:
    Direction direction = Direction::UP;

    Texture* images[4];
};

