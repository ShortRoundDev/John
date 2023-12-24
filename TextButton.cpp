#include "TextButton.h"

#include <Windows.h>

#include "App.h"

TextButton::TextButton(std::string* text) : UINode(
	{
		24, 0,
		22, 22,
		"Resources/UI/TextButton",
		CGA_TRANSPARENT,
		StyleDirection::TOP,
		StyleDirection::RIGHT
	}
)
{
	this->text = text;
	depressed = image;

	if (!APP->tryLoadTexture("Resources/UI/TextButtonPressed.png", "TextButtonPressed", &pressed))
	{
		MessageBoxA(NULL, "Failed to load Textbuttonpressed!", NULL, MB_OK);
		return;
	}
}

bool TextButton::onMouseDown(const SDL_Event& e)
{
	image = pressed;
	return true;
}

bool TextButton::onMouseUp(const SDL_Event& e)
{
	APP->showTextBox(text);
	image = depressed;
	return true;
}