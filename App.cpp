#include "App.h"

#include <iostream>
#include <Windows.h>

#include "UINode.h"

#include "Grid.h"

#include "UIButton.h"
#include "SaveButton.h"
#include "LoadButton.h"

#include "SectionHeader.h"
#include "Tile.h"
#include "TileArea.h"
#include "EntityArea.h"
#include "PropertiesEditor.h"

#include "TextBox.h"

std::unique_ptr<App> App::instance = nullptr;

void App::init(std::string title)
{
	instance = std::make_unique<App>(title);
	instance->createDom();

	if (!instance->tryLoadTexture("Resources/UI/Numbers.png", "Numbers", &instance->numbers))
	{
		MessageBoxA(NULL, "Failed to load numbers font file", NULL, MB_OK);
	}
	if (!instance->tryLoadTexture("Resources/UI/Font.png", "Letters", &instance->letters))
	{
		MessageBoxA(NULL, "Failed to load letters font file", NULL, MB_OK);
	}
}

App::App(std::string title)
{
	window = NULL;
	renderer = NULL;

	width = 1024;
	height = 768;

	SDL_CreateWindowAndRenderer(
		1024,
		768,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
		&window,
		&renderer
	);
	SDL_SetWindowTitle(window, title.c_str());

	auto mouse = IMG_Load("Resources/UI/mouse.png");
	auto cursor = SDL_CreateColorCursor(mouse, 0, 0);
	SDL_SetCursor(cursor);
}

App::~App()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;
}

void App::createDom()
{
	rootDom = new UINode(
		{
			0, 0,
			UINode::PCT(100), UINode::PCT(100)
		},
		3,
		Grid::init(),
		new UINode(
			{
				0, 0,
				256, UINode::PCT(100),
				"",
				CGA_LT_BLUE,
				StyleDirection::TOP,
				StyleDirection::RIGHT,
			},
			6,
			new SectionHeader(
				64, 48,
				"Resources/UI/Tiles"
			),
			new TileArea(),
			new SectionHeader(
				64, 256 + 112,
				"Resources/UI/Entities"
			),
			new EntityArea,
			new SectionHeader(
				64, 512 + 112 + 64,
				"Resources/UI/Properties"
			),
			PropertiesEditor::init()
		),
		new UINode(
			{
				0, 0,
				UINode::PCT(100), 32,
				"",
				CGA_LT_BLUE,
				StyleDirection::TOP,
				StyleDirection::LEFT,
				CGA_CYAN
			},
			2,
			new SaveButton(),
			new LoadButton()
		)
	);
}

void App::update()
{

}

void App::draw()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 170, 255);
	SDL_RenderClear(renderer);

	rootDom->draw({ 0, 0, width, height});
	if (textbox != nullptr)
	{
		textbox->draw({ 0, 0, width, height });
	}

	SDL_RenderPresent(renderer);
}

void App::events()
{
	SDL_Event e = { 0 };
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
		{
			shouldQuit = true;
			return;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			lastMouseDown = { e.button.x, e.button.y };
			switch(e.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				mouseState |= MOUSE_LEFT;
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				mouseState |= MOUSE_MIDDLE;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				mouseState |= MOUSE_RIGHT;
				break;
			}
			}

			if (textbox != nullptr)
			{
				textbox->handleMouseDown({ 0, 0, width, height }, e);
			}
			else
			{
				rootDom->handleMouseDown({ 0, 0, width, height }, e);
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			switch (e.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				mouseState &= ~MOUSE_LEFT;
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				mouseState &= ~MOUSE_MIDDLE;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				mouseState &= ~MOUSE_RIGHT;
				break;
			}
			}

			dragging = false;
			if (textbox != nullptr)
			{
				textbox->handleMouseUp({ 0, 0, width, height }, e);
				if (closeTextbox)
				{
					delete textbox;
					textbox = nullptr;
					closeTextbox = false;
				}
			}
			else
			{
				rootDom->handleMouseUp({ 0, 0, width, height }, e);
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			if (mouseState != 0 && !dragging)
			{
				SDL_Point now = { e.button.x, e.button.y };
				auto deltaX = now.x - lastMouseDown.x;
				auto deltaY = now.y - lastMouseDown.y;
				if (sqrt((deltaX * deltaX) + (deltaY * deltaY)) > 10)
				{
					dragging = true;
				}
			}
			if (dragging)
			{
				if (textbox != nullptr)
				{
					textbox->handleDrag({ 0, 0, width, height }, e);
				}
				else
				{
					rootDom->handleDrag({ 0, 0 }, e);
				}
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			if (textbox != nullptr)
			{
				textbox->handleMouseScroll({ 0, 0, width, height }, e);
			}
			else
			{
				rootDom->handleMouseScroll({ 0, 0 }, e);
			}
			break;
		}
		case SDL_KEYDOWN:
		{
			keymap[e.key.keysym.scancode] = true;
			if (textbox == nullptr)
			{
				rootDom->handleKeyDown(e);
			}
			else
			{
				textbox->handleKeyDown(e);
			}
			if (e.key.keysym.scancode == SDL_SCANCODE_F11)
			{
				auto current = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
				SDL_SetWindowFullscreen(window, current ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			break;
		}
		case SDL_KEYUP:
		{
			keymap[e.key.keysym.scancode] = false;
			if (textbox == nullptr)
			{
				rootDom->handleKeyUp(e);
			}
			break;
		}
		case SDL_TEXTINPUT:
		{
			if (textbox != nullptr)
			{
				TextBox* box = ((TextBox*)textbox);
				std::string* textPtr = box->text;
				if (textPtr->length() >= 1023)
					break;
				std::cout << textPtr->length() << std::endl;				
				*textPtr = textPtr->substr(0, box->cursorPos) + e.text.text[0] + textPtr->substr(box->cursorPos, textPtr->length());
				box->cursorPos++;
			}
			break;
		}
		case SDL_WINDOWEVENT:
		{
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			{
				windowResizeEvent(e);
				break;
			}
			}
			break;
		}
		}
	}
}

void App::windowResizeEvent(const SDL_Event& e)
{
	SDL_GetWindowSize(window, &width, &height);
}

_Success_(return)
bool App::tryLoadTexture(_In_ std::string path, _In_ std::string alias, _Out_ Texture** texture)
{
	Texture* t = NULL;
	if (!Texture::tryLoad(path, &t))
	{
		*texture = NULL;
		return false;
	}

	textures[alias] = t;

	*texture = t;
	return true;
}

void App::drawText(int x, int y, const char* text)
{
	int i = 0;
	for (const char* c = text; *c != 0; c++, i++)
	{
		int sy = 0;
		int sx = *c;
		SDL_Rect src = {
			sx * 14, sy * 18,
			14, 20
		};
		SDL_Rect dst = {
			x + (i * 15), y - 16,
			28, 40
		};
		SDL_RenderCopy(renderer, letters->texture, &src, &dst);
	}
}

void App::drawTextBox(const SDL_Rect& container, const char* text, int cursor)
{
	int pos = 0;
	int i = 0;
	int dx = 0;
	int dy = 0;
	int x = container.x;
	int y = container.y;
	for (const char* c = text; *c != 0; c++, i++, pos++)
	{
		x = container.x;
		y = container.y;

		dx = (i * 18);
		if (dx > container.w - 14)
		{
			dx = 0;
			i = 0;
			dy += 22;
		}

		if (*c == '\n')
		{
			i = -1;
			dx = 0;
			dy += 22;
			continue;
		}

		int sy = 0;
		int sx = *c;

		SDL_Rect src = {
			sx * 14, sy * 18,
			14, 20
		};
		SDL_Rect dst = {
			x + dx - 8, y + dy - 12,
			28, 40
		};

		if (pos == cursor)
		{
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			SDL_Rect line = {
				x + dx - 2, y + dy - 4,
				2, 19 + 8
			};
			SDL_RenderFillRect(renderer, &line);
		}

		SDL_RenderCopy(renderer, letters->texture, &src, &dst);
	}
	if (cursor == strlen(text))
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_Rect line = {
			x + dx - 2 + 14, y + dy - 4,
			2, 19 + 8
		};
		SDL_RenderFillRect(renderer, &line);
	}
}

void App::showTextBox(std::string* text)
{
	this->textbox = new TextBox(text);
}
