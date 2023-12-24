#pragma once

#include "UINode.h"

class EntityButton : public UINode
{
public:
	EntityButton
	(
		int tileOrder,
		int entId,
		Texture* entTexture,
		std::string entName
	);

	virtual void draw(const SDL_Rect& container);
	bool onMouseDown(const SDL_Event& e);
	bool onMouseUp(const SDL_Event& e);

	int entId;
	std::string entName;
	Texture* entTexture;
private:

	Texture* pressed;
	Texture* depressed;
};

