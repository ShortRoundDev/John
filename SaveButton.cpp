#include "SaveButton.h"

#include "FileHandling.h"
#include "Grid.h"
#include <iostream>

SaveButton::SaveButton() : UIButton(
	{
		0, 0,
		128, 32,
		"Resources/UI/Save"
	}
)
{

}

bool SaveButton::onMouseUp(const SDL_Event& e)
{

	UIButton::onMouseUp(e);

	std::wstring path = getFilePath();
	
	GRID->save(path);

	return true;
}