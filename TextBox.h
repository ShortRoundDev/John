#pragma once

#include "UINode.h"

#include <optional>
#include <functional>

class TextBox : public UINode
{
public:
	TextBox(std::string* text);
	TextBox(std::string initialValue, std::function<void(std::string)> onSave);

	~TextBox();

	void draw(const SDL_Rect& container);

	bool onKeyDown(const SDL_Event& e) override;
	bool onMouseDown(const SDL_Event& e) override;
	bool onTextInput(const SDL_Event& e) override;

	int cursorPos = 0;

	std::string* text;
	std::string value;
	std::optional<std::function<void(std::string)>> onSave;

private:
	TextBox(std::string value, std::string* text, std::optional<std::function<void(std::string)>> onSave);
};

