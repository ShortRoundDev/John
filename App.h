#pragma once

#include <string>
#include <map>
#include <memory>
#include <functional>
#include <optional>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Texture.h"
#include "EntityMetadata.h"

#define APP (App::instance)

// Mouse constexpr
constexpr uint8_t MOUSE_LEFT = 1;
constexpr uint8_t MOUSE_MIDDLE = 2;
constexpr uint8_t MOUSE_RIGHT = 4;

/* Forward declaration */
class UINode;
class EntityButton;

class App
{
public:
	static std::unique_ptr<App> instance;
	static void init(std::string title);

	App(std::string title);
	~App();

	void createDom();

	SDL_Window* window;
	SDL_Renderer* renderer;

	std::map<std::string, Texture*> textures;
	std::map<uint16_t, std::string> idTextureMapping;
	std::map<uint16_t, std::vector<EntityMetadataField>> entityMetadata;
	std::map<SDL_Scancode, bool> keymap;
 
	Texture* numbers;
	Texture* letters;
	
	bool shouldQuit = false;

	UINode* rootDom;

	void update();
	void draw();
	void events();
	// event stuff
	uint8_t mouseState; // 1 = left, 2 = middle, 4 = right
	bool dragging = false;
	SDL_Point lastMouseDown;

	void windowResizeEvent(const SDL_Event& e);

	_Success_(return)
	bool tryLoadTexture(_In_ std::string path, _In_ std::string alias, _Out_ Texture** texture);

	_Success_(return)
	bool tryLoadEntityConfig(std::string name, EntityMetadata& metadata);

	int width;
	int height;

	// ----- Editor stuff -----
	int selectedTile = 0;
	Texture* selectedTileTexture = nullptr;

	EntityButton* selectedEntity = nullptr;

	void drawText(int x, int y, const char* text);
	void drawTextBox(const SDL_Rect& container, const char* text, int cursor, bool drawCursor = true, bool wrap = true);

	bool closeTextbox = false;
	void showTextBox(std::string* text);
	void showTextBox(std::string initialValue, std::function<void(std::string)> onSave);

	void setActive(UINode* activeInputText);
	bool isActive(UINode* node);

private:
	UINode* activeInputText = nullptr;
};

