#pragma once

#include "UINode.h"

#include <optional>
#include <functional>

class TextButton : public UINode
{
public:
	TextButton(std::string* text);
	TextButton(std::string initialValue, std::function<void(std::string)> onSave);

	Texture* pressed;
	Texture* depressed;

	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

	std::string* text;
	std::string value;
	std::optional<std::function<void(std::string)>> onSave;

private:
	TextButton(std::string value, std::string* text, std::optional<std::function<void(std::string)>> onSave);
};

