#pragma once

#include <string>
#include <map>
#include <vector>
#include <SDL2/SDL.h>

#include "Texture.h"

//Dark
constexpr SDL_Color CGA_BLACK	   = { 0x00, 0x00, 0x00, 0xff };
constexpr SDL_Color CGA_BLUE	   = { 0x00, 0x00, 0xaa, 0xff };
constexpr SDL_Color CGA_GREEN	   = { 0x00, 0xaa, 0x00, 0xff };
constexpr SDL_Color CGA_CYAN	   = { 0x00, 0xaa, 0xaa, 0xff };
constexpr SDL_Color CGA_RED		   = { 0xaa, 0x00, 0x00, 0xff };
constexpr SDL_Color CGA_MAGENTA    = { 0xaa, 0x00, 0xaa, 0xff };
constexpr SDL_Color CGA_BROWN	   = { 0xaa, 0x55, 0x00, 0xff };
constexpr SDL_Color CGA_LT_GRAY    = { 0xaa, 0xaa, 0xaa, 0xff };
// Light

constexpr SDL_Color CGA_DK_GRAY    = { 0x55, 0x55, 0x55, 0xff };
constexpr SDL_Color CGA_LT_BLUE    = { 0x55, 0x55, 0xff, 0xff };
constexpr SDL_Color CGA_LT_GREEN   = { 0x55, 0xff, 0x55, 0xff };
constexpr SDL_Color CGA_LT_CYAN	   = { 0x55, 0xff, 0xff, 0xff };
constexpr SDL_Color CGA_LT_RED	   = { 0xff, 0x55, 0x55, 0xff };
constexpr SDL_Color CGA_LT_MAGENTA = { 0xff, 0x55, 0xff, 0xff };
constexpr SDL_Color CGA_YELLOW	   = { 0xff, 0xff, 0x55, 0xff };
constexpr SDL_Color CGA_WHITE	   = { 0xff, 0xff, 0xff, 0xff };

constexpr SDL_Color CGA_TRANSPARENT = { 0x00, 0x00, 0x00, 0x00 };

constexpr uint16_t PCT_BIT = 0x8000;

enum class StyleDirection
{
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
};

typedef struct _Style
{
	uint16_t x = 0;
	uint16_t y = 0;
	
	uint16_t width = 100 & PCT_BIT;
	uint16_t height = 32;
	
	std::string texturePath;
	SDL_Color color = { 0, 0, 0, 0 };

	StyleDirection yAnchor = StyleDirection::TOP;
	StyleDirection xAnchor = StyleDirection::LEFT;

	SDL_Color border = { 0, 0, 0, 0 };

	bool overflow = true;
	bool scrollable = false;
} Style;

class UINode
{
public:
	static uint16_t PCT(uint16_t x);
	static uint16_t calcX(uint16_t x);
	static uint16_t calcY(uint16_t y);

	UINode(Style style);
	UINode(Style style, int totalChildren, ...);

	virtual ~UINode();
	
	virtual void draw(const SDL_Rect& container);
	virtual void update();
	
	bool handleMouseDown(SDL_Rect container, const SDL_Event& e);
	virtual bool onMouseDown(const SDL_Event& e);

	bool handleDrag(SDL_Rect container, const SDL_Event& e);
	virtual bool onDrag(const SDL_Event& e);
	
	bool handleMouseUp(SDL_Rect container, const SDL_Event& e);
	virtual bool onMouseUp(const SDL_Event& e);

	bool handleMouseScroll(SDL_Rect container, const SDL_Event& e);

	bool handleKeyDown(const SDL_Event& e);
	virtual bool onKeyDown(const SDL_Event& e);

	bool handleKeyUp(const SDL_Event& e);
	virtual bool onKeyUp(const SDL_Event& e);

	bool handleTextInput(const SDL_Event& e);
	virtual bool onTextInput(const SDL_Event& e);

	bool isActive();

	std::vector<UINode*> children;
	
	Style style;

	int16_t scroll = 0;

	void deleteChild(UINode* child, bool recursive = true);
	void addChild(UINode* child);

	void checkAddRemove();

protected:
	Texture* image;

	SDL_Point getDrawOffset(const SDL_Rect& container);

	std::vector<UINode*> toAdd;
	std::vector<UINode*> toRemove;
};

