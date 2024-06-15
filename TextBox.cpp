#include "TextBox.h"

#include "CloseTextBoxButton.h"

#include "App.h"

TextBox::TextBox(std::string* text) : TextBox(*text, text, std::nullopt) { }
TextBox::TextBox(std::string initialValue, std::function<void(std::string)> onSave) : TextBox(initialValue, nullptr, onSave) { }

TextBox::TextBox(std::string value, std::string* text, std::optional<std::function<void(std::string)>> onSave) :
	UINode({
		256 + 16, 16,
		512, 320,
		"Resources/UI/TextBox",
		CGA_TRANSPARENT,
		StyleDirection::BOTTOM,
		StyleDirection::RIGHT,
		CGA_TRANSPARENT,
		false, true
	}),
	text(text), value(value), onSave(onSave)
{
	addChild(new CloseTextBoxButton(this));
	APP->setActive(this);
}


TextBox::~TextBox()
{
	UINode::~UINode();
}

void TextBox::draw(const SDL_Rect& container)
{
	int tmp = scroll;
	scroll = 0;
	UINode::draw(container);
	scroll = tmp;

	auto offset = getDrawOffset(container);
	SDL_Rect box = {
		offset.x + 10, offset.y + 32,
		calcX(style.width) - 26, calcY(style.height) - 48
	};

	SDL_RenderSetClipRect(APP->renderer, &box);
	box.y -= scroll;
	APP->drawTextBox(box, value.c_str(), cursorPos);
	SDL_Rect full = {
		0, 0, APP->width, APP->height
	};
	SDL_RenderSetClipRect(APP->renderer, &full);
}

bool TextBox::onTextInput(const SDL_Event& e)
{
    value = value.substr(0, cursorPos) + e.text.text + value.substr(cursorPos, value.length());
    cursorPos++;
	if (onSave.has_value())
	{
        onSave.value()(value);
    }
	else if (text != nullptr)
	{
        *text = value;
    }
    return true;
}

bool TextBox::onKeyDown(const SDL_Event& e)
{
	if (!isActive())
	{
		return true;
	}
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_LEFT:
		cursorPos--;
		break;
	case SDL_SCANCODE_RIGHT:
		cursorPos++;
		break;
	case SDL_SCANCODE_BACKSPACE:
		if (value.length() > 0 && cursorPos > 0)
		{
			value = value.substr(0, cursorPos - 1) + value.substr(cursorPos, value.length());
			cursorPos--;
		}
		break;
	case SDL_SCANCODE_RETURN:
		value = value.substr(0, cursorPos) + "\n" + value.substr(cursorPos, value.length());
		cursorPos++;
		break;
	}

	if (cursorPos < 0)
		cursorPos = 0;
	if (cursorPos > value.length())
		cursorPos = (int)value.length();

	if (onSave.has_value())
	{
		onSave.value()(value);
	}
	else if(text != nullptr)
    {
        *text = value;
    }
	return true;
}

bool TextBox::onMouseDown(const SDL_Event& e)
{
	APP->setActive(this);
	return false;
}