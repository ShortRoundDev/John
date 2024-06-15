#pragma once

#include "UINode.h"
#include <functional>
#include <optional>

class Checkbox : public UINode
{
public:
	Checkbox(bool* toCheck);
	Checkbox(bool initialValue, std::function<void(bool)> onClick);

	Texture* checkDepressed;
	Texture* checkPressed;

	Texture* uncheckDepressed;
	Texture* uncheckPressed;

	bool* toCheck;
	bool value;

	std::optional<std::function<void(bool)>> onClick;

	bool onMouseUp(const SDL_Event& e);
	bool onMouseDown(const SDL_Event& e);

private:
	void init(bool initialValue);
};

