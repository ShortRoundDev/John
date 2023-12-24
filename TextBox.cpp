#include "TextBox.h"

#include "CloseTextBoxButton.h"

#include "App.h"

TextBox::TextBox(std::string* text) : UINode(
	{
		256 + 16, 16,
		512, 320,
		"Resources/UI/TextBox",
		CGA_TRANSPARENT,
		StyleDirection::BOTTOM,
		StyleDirection::RIGHT,
		CGA_TRANSPARENT,
		false, true
	}
)
{
	this->text = text;
	children.push_back(new CloseTextBoxButton());
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
		offset.x + 12, offset.y + 32,
		calcX(style.width) - 32, calcY(style.height) - 48
	};

	SDL_RenderSetClipRect(APP->renderer, &box);
	box.y -= scroll;
	APP->drawTextBox(box, text->c_str(), cursorPos);
	SDL_Rect full = {
		0, 0, APP->width, APP->height
	};
	SDL_RenderSetClipRect(APP->renderer, &full);
}

bool TextBox::onKeyDown(const SDL_Event& e)
{
	switch (e.key.keysym.scancode)
	{
	case SDL_SCANCODE_LEFT:
		cursorPos--;
		break;
	case SDL_SCANCODE_RIGHT:
		cursorPos++;
		break;
	case SDL_SCANCODE_BACKSPACE:
		if (text->length() > 0 && cursorPos > 0)
		{
			*text = text->substr(0, cursorPos - 1) + text->substr(cursorPos, text->length());
			cursorPos--;
		}
		break;
	case SDL_SCANCODE_RETURN:
		*text = text->substr(0, cursorPos) + "\n" + text->substr(cursorPos, text->length());
		cursorPos++;
		break;
	}

	if (cursorPos < 0)
		cursorPos = 0;
	if (cursorPos > text->length())
		cursorPos = (int)text->length();
	return true;
}