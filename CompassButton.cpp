#include "CompassButton.h"
#include "App.h"
#include <iostream>
#include "Grid.h"

CompassButton::CompassButton() : UINode({
    51 * 2, 4 * 2,
    32, 32,
    "Resources/UI/Compass_Up",
    CGA_TRANSPARENT,
    StyleDirection::TOP,
    StyleDirection::LEFT
})
{
    on = false;
    up = image;
    if (!APP->tryLoadTexture("Resources/UI/Compass_Down.png", "Compass_Down", &this->down))
    {
        std::cout << "Failed to load compass down!" << std::endl;
        std::cout << SDL_GetError() << std::endl;
        return;
    }

}


void CompassButton::draw(const SDL_Rect& container)
{
    UINode::draw(container);
}

bool CompassButton::onMouseDown(const SDL_Event& e)
{
    on ^= true;
    if (on)
    {
        image = down;
    }
    else
    {
        image = up;
    }
    GRID->setRotateVisible(on);
    return false;
}