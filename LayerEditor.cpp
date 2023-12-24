#include "LayerEditor.h"

#include "App.h"
#include "LayerIncrementorButton.h"
#include "Grid.h"

LayerEditor::LayerEditor(Grid* grid) : UINode(
	{
		256 + 16, 16,
		128, 64,
		"Resources/UI/LayerEditor",
		CGA_TRANSPARENT,
		StyleDirection::TOP,
		StyleDirection::RIGHT
	})
{
	this->grid = grid;
	children.push_back(
		new LayerIncrementorButton(
			(uint16_t)64, (uint16_t)6,
			true,
			[grid]()
			{
				grid->changeLayer(1);
			}
	));

	children.push_back(
		new LayerIncrementorButton(
			(uint16_t)108, (uint16_t)6,
			false,
			[grid]()
			{
				grid->changeLayer(-1);
			}
	));

	children.push_back(
		new LayerIncrementorButton(
			(uint16_t)64, (uint16_t)36,
			true,
			[grid]()
			{
				grid->changeZone(1);
			}
	));

	children.push_back(
		new LayerIncrementorButton(
			(uint16_t)108, (uint16_t)36,
			false,
			[grid]()
			{
				grid->changeZone(-1);
			}
	));
}

LayerEditor::~LayerEditor()
{

}

void LayerEditor::draw(const SDL_Rect& container)
{
	// draw layer number
	UINode::draw(container);
	SDL_Point offset = getDrawOffset(container);

	SDL_Rect src = {
		grid->getCurrentLayer() * 14, 0,
		14, 18
	};

	SDL_Rect dst = {
		offset.x + 86, offset.y + 8,
		14, 18
	};

	SDL_RenderCopy(APP->renderer, APP->numbers->texture, &src, &dst);

	src = {
		grid->getCurrentZone() * 14, 0,
		14, 18
	};

	dst = {
		offset.x + 86, offset.y + 38,
		14, 18
	};

	SDL_RenderCopy(APP->renderer, APP->numbers->texture, &src, &dst);
}

bool LayerEditor::onMouseDown(const SDL_Event& e)
{
	return false;
}

bool LayerEditor::onMouseUp(const SDL_Event& e)
{
	return false;
}