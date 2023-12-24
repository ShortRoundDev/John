#include "PropertyLine.h"

#include "App.h"

PropertyLine::PropertyLine(uint16_t y, std::string name, UINode* button) : UINode(
	{
		0, (uint16_t)(y * 48 + 8),
		224, 48
	}
)
{
	this->name = name;
	children.push_back(button);
}

PropertyLine::~PropertyLine()
{

}

void PropertyLine::draw(const SDL_Rect& container)
{
	UINode::draw(container);
	SDL_Point start = getDrawOffset(container);

	APP->drawText(start.x + 4, start.y + 4, name.c_str());
}