#include "LoadButton.h"

#include "FileHandling.h"
#include "MapLoader.h"

LoadButton::LoadButton() : UIButton(
	{
		128, 0,
		128, 32,
		"Resources/UI/Load"
	}
)
{

}

bool LoadButton::onMouseUp(const SDL_Event& e)
{
	UIButton::onMouseUp(e);

	auto path = getOpenFilePath();
	MapLoader* loader = new MapLoader;
	loader->loadMap(path);

	return true;
}