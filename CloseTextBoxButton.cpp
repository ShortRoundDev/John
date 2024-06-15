#include "CloseTextBoxButton.h"

#include <Windows.h>

#include "App.h"

CloseTextBoxButton::CloseTextBoxButton(UINode* parent) :
	UINode(
		{
			8, 4,
			18, 18,
			"Resources/UI/CloseButton",
			CGA_TRANSPARENT,
			StyleDirection::TOP,
			StyleDirection::RIGHT
		}
	),
	parent(parent)
{
	depressed = image;
	if (!APP->tryLoadTexture("Resources/UI/CloseButtonPressed.png", "CloseButtonPressed", &pressed))
	{
		MessageBoxA(NULL, "Failed to lose CloseButtonPressed", NULL, MB_OK);
		return;
	}
}

CloseTextBoxButton::~CloseTextBoxButton()
{

}

bool CloseTextBoxButton::onMouseDown(const SDL_Event& e)
{
	image = pressed;
	return true;
}

bool CloseTextBoxButton::onMouseUp(const SDL_Event& e)
{
	image = depressed;
	APP->rootDom->deleteChild(parent);
	APP->setActive(nullptr);
	return true;
}