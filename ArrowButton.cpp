#include "ArrowButton.h"
#include "App.h"
#include "Grid.h"

#include <iostream>

ArrowButton::ArrowButton() : UINode({
    64, 8,
    32, 32,
    "Resources/UI/Arrow_Up",
    CGA_TRANSPARENT,
    StyleDirection::TOP,
    StyleDirection::LEFT
})
{
    this->images[0] = image;
    if (!APP->tryLoadTexture("Resources/UI/Arrow_Right.png", "Arrow_Right", &this->images[1]) ||
        !APP->tryLoadTexture("Resources/UI/Arrow_Down.png", "Arrow_Down", &this->images[2]) ||
        !APP->tryLoadTexture("Resources/UI/Arrow_Left.png", "Arrow_Left", &this->images[3])
    )
    {
        std::cout << "Failed to load arrow images!" << std::endl;
        return;
    }
}

bool ArrowButton::onMouseDown(const SDL_Event& e)
{
    switch (direction)
    {
    case Direction::UP:
        direction = Direction::RIGHT;
        break;
    case Direction::RIGHT:
        direction = Direction::DOWN;
        break;
    case Direction::DOWN:
        direction = Direction::LEFT;
        break;
    case Direction::LEFT:
        direction = Direction::UP;
        break;
    }

    GRID->setRotation(static_cast<int>(direction));

    this->image = images[(static_cast<int>(this->direction))];
    return false;
}