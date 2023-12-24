#include "LayerIncrementorButton.h"

#include <Windows.h>

#include "App.h"


LayerIncrementorButton::LayerIncrementorButton(uint16_t x, uint16_t y, bool up, std::function<void()> callback) : UINode(
	{
		x, y,
		14, 22,
		up ? "Resources/UI/Up" : "Resources/UI/Down"
	}
)
{
	this->increment = callback;
	this->depressed = image;
	std::string icon = up ? "Resources/UI/Up" : "Resources/UI/Down";
	icon += "Pressed";
	if(!APP->tryLoadTexture(icon + ".png", icon, &pressed))
	{
		MessageBoxA(NULL, ("Failed to load " + icon).c_str(), NULL, MB_OK);
		return;
	}
}

bool LayerIncrementorButton::onMouseDown(const SDL_Event& e)
{
	this->image = pressed;
	return false;
}

bool LayerIncrementorButton::onMouseUp(const SDL_Event& e)
{
	this->image = depressed;
	this->increment();
	return false;
}