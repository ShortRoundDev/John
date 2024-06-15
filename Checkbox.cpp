#include "Checkbox.h"

#include "App.h"
#include "Windows.h"

Checkbox::Checkbox(bool* toCheck) : UINode(
	{
		24, 0,
		22, 22,
		"",
		CGA_TRANSPARENT,
		StyleDirection::TOP,
		StyleDirection::RIGHT
	}
)
{
	this->toCheck = toCheck;
	onClick = std::nullopt;

	init(*toCheck);
}

Checkbox::Checkbox(bool initialValue, std::function<void(bool)> onClick) : UINode(
	{
		24, 0,
		22, 22,
		"",
		CGA_TRANSPARENT,
		StyleDirection::TOP,
		StyleDirection::RIGHT
	}
)
{
    this->onClick = onClick;
	toCheck = nullptr;

	init(initialValue);
}

bool Checkbox::onMouseDown(const SDL_Event& e)
{
	image = value
		? checkPressed
		: uncheckPressed;
	return true;
}

bool Checkbox::onMouseUp(const SDL_Event& e)
{
	if (toCheck)
	{
		*toCheck ^= true;
		value = *toCheck;
	}
	else if(onClick.has_value())
	{
		value ^= true;
		onClick.value()(value);
	}

	image = value
		? checkDepressed
		: uncheckDepressed;
	return true;
}

void Checkbox::init(bool initialValue)
{
	value = initialValue;
	if (
		!APP->tryLoadTexture("Resources/UI/CheckBoxUnchecked.png", "CheckboxUnchecked", &uncheckDepressed) ||
		!APP->tryLoadTexture("Resources/UI/CheckBoxUncheckedPressed.png", "CheckboxUncheckedPressed", &uncheckPressed) ||
		!APP->tryLoadTexture("Resources/UI/CheckBoxChecked.png", "CheckboxChecked", &checkDepressed) ||
		!APP->tryLoadTexture("Resources/UI/CheckBoxCheckedPressed.png", "CheckboxCheckedPressed", &checkPressed)
		)
	{
		MessageBoxA(NULL, "Failed to initialize checkbox images", NULL, MB_OK);
		return;
	}


	image = initialValue
		? checkDepressed
		: uncheckDepressed;
}