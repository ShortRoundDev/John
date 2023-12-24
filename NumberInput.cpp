#include "NumberInput.h"

#include "LayerIncrementorButton.h"

#include "App.h"

NumberInput::NumberInput(uint8_t* number) : UINode({
	0, 0,
	128, 32,
	"Resources/UI/NumberInput",
	CGA_TRANSPARENT,
	StyleDirection::TOP,
	StyleDirection::RIGHT
})
{
	this->number = number;
	children.push_back(
		new LayerIncrementorButton(65, 4, true,
			[this]()
			{
				if (*this->number == 9)
					return;
				(*this->number)++;
			}
		)
	);
	children.push_back(
		new LayerIncrementorButton(106, 4, false,
			[this]()
			{
				if (*this->number == 0)
					return;
				(*this->number)--;
			}
		)
	);
}

NumberInput::~NumberInput()
{
	UINode::~UINode();
}

void NumberInput::draw(const SDL_Rect& container)
{
	UINode::draw(container);
	SDL_Point offset = getDrawOffset(container);

	SDL_Rect src = {
		*number * 14, 0,
		14, 18
	};

	SDL_Rect dst = {
		offset.x + 86, offset.y + 6,
		14, 18
	};

	SDL_RenderCopy(APP->renderer, APP->numbers->texture, &src, &dst);
}