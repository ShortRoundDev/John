#include "TextButton.h"

#include <Windows.h>

#include "App.h"

TextButton::TextButton(std::string* text) :
	TextButton(
		*text,
		text,
		std::nullopt
	)
{
}

TextButton::TextButton(std::string initialValue, std::function<void(std::string)> onSave) : TextButton(
	initialValue,
	nullptr,
	onSave
){ }

TextButton::TextButton(std::string value, std::string* text, std::optional<std::function<void(std::string)>> onSave) :
	UINode(
		{
			24, 0,
			22, 22,
			"Resources/UI/TextButton",
			CGA_TRANSPARENT,
			StyleDirection::TOP,
			StyleDirection::RIGHT
		}
	),
	value(value),
	text(text),
	onSave(onSave)
{
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
	if (text != nullptr)
	{
		APP->showTextBox(text);
	}
	else if (onSave.has_value())
	{
		APP->showTextBox(value, onSave.value());
	}
	image = depressed;
	return true;
}

