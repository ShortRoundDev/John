#include "UIButton.h"
#include "App.h"

#include "Windows.h"

UIButton::UIButton(Style style) :
	UINode(style)
{
	if (!APP->tryLoadTexture(style.texturePath + "Pressed.png", style.texturePath + "Pressed", &pressed))
	{
		MessageBoxA(NULL, ("Failed to load [" + style.texturePath + "Pressed.png]").c_str(), NULL, MB_OK);
	}

	depressed = image;
}

UIButton::~UIButton()
{

}

bool UIButton::onMouseDown(const SDL_Event& e)
{
	image = pressed;
	return true;
}

bool UIButton::onMouseUp(const SDL_Event& e)
{
	image = depressed;
	return true;
}