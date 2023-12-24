#include "UINode.h"
#include "App.h"

#include "Windows.h"

uint16_t UINode::PCT(uint16_t x)
{
	return x | PCT_BIT;
}

uint16_t UINode::calcX(uint16_t x)
{
	return (x & PCT_BIT)
		? (uint16_t)(APP->width * ((float)(x & ~PCT_BIT) / 100.0f))
		: x;
}

uint16_t UINode::calcY(uint16_t y)
{
	return (y & PCT_BIT)
		? (uint16_t)(APP->height * ((float)(y & ~PCT_BIT) / 100.0f))
		: y;
}


UINode::UINode(Style style)
{
	image = NULL;
	this->style = style;

	if (!style.texturePath.empty())
	{
		if (!APP->tryLoadTexture(style.texturePath + ".png", style.texturePath, &image))
		{
			MessageBoxA(NULL, ("Failed to load image [" + style.texturePath + "]").c_str(), NULL, MB_OK);
		}
	}

}

UINode::UINode(Style style, int totalChildren, ...) : UINode(style)
{
	va_list args;
	UINode* children = NULL;
	va_start(args, totalChildren);
	for (int i = 0; i < totalChildren; i++)
	{
		auto child = va_arg(args, UINode*);
		this->children.push_back(child);
	}
}

UINode::~UINode()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
		children[i] = nullptr;
	}
	children.clear();
}

void UINode::draw(const SDL_Rect& container)
{
	SDL_Point offset = getDrawOffset(container);

	SDL_Rect _dimensions
	{
		offset.x,
		offset.y,
		calcX(style.width),
		calcY(style.height),
	};
	
	if (style.border.a != 0)
	{
		SDL_RenderSetScale(APP->renderer, 2.0f, 2.0f);

		SDL_SetRenderDrawColor(APP->renderer, style.border.r, style.border.g, style.border.b, style.border.a);

		SDL_Rect borderDimensions = {
			_dimensions.x/2 - 1, _dimensions.y/2 - 1,
			_dimensions.w/2 + 2, _dimensions.h/2 + 2
		};
		SDL_RenderDrawRect(APP->renderer, &borderDimensions);
		SDL_RenderSetScale(APP->renderer, 1.0f, 1.0f);
	}

	if (!style.overflow)
	{
		SDL_RenderSetClipRect(APP->renderer, &_dimensions);
	}

	if (style.color.a != 0)
	{
		SDL_SetRenderDrawColor(APP->renderer, style.color.r, style.color.g, style.color.b, style.color.a);
		SDL_RenderFillRect(
			APP->renderer,
			&_dimensions
		);
	}

	if (style.scrollable)
	{
		_dimensions.y -= scroll;
	}

	if (image != NULL)
	{
		SDL_RenderCopy(
			APP->renderer,
			image->texture,
			&(image->src),
			&_dimensions
		);
	}

	for (const auto& c : children)
	{
		c->draw(_dimensions);
	}
	if (!style.overflow)
	{
		SDL_Rect fullWindow = { 0, 0, APP->width, APP->height };
		SDL_RenderSetClipRect(APP->renderer, &fullWindow);
	}
}

void UINode::update()
{

}

bool UINode::handleMouseDown(SDL_Rect container, const SDL_Event& e)
{
	auto offset = getDrawOffset(container);

	SDL_Rect transformedDim = {
		offset.x,
		offset.y,
		calcX(style.width),
		calcY(style.height)
	};

	SDL_Point mouse = {
		e.button.x,
		e.button.y
	};

	if (SDL_PointInRect(&mouse, &transformedDim))
	{
		bool bubble = true;
		for (const auto& c : children)
		{
			bubble &= c->handleMouseDown({ transformedDim.x, transformedDim.y - scroll, transformedDim.w, transformedDim.h }, e);
		}

		if (bubble)
		{
			bubble &= onMouseDown(e);
		}

		return bubble;
	}
	return true;
}

bool UINode::onMouseDown(const SDL_Event& e)
{
	return true;
}

bool UINode::handleDrag(SDL_Rect container, const SDL_Event& e)
{
	auto offset = getDrawOffset(container);

	SDL_Rect transformedDim = {
		offset.x,
		offset.y,
		calcX(style.width),
		calcY(style.height)
	};

	SDL_Point mouse = {
		e.button.x,
		e.button.y
	};

	if (SDL_PointInRect(&mouse, &transformedDim))
	{
		bool bubble = true;
		for (const auto& c : children)
		{
			bubble &= c->handleDrag({ transformedDim.x, transformedDim.y - scroll, transformedDim.w, transformedDim.h }, e);
		}

		if (bubble)
		{
			bubble &= onDrag(e);
		}

		return bubble;
	}
	return true;
}

bool UINode::onDrag(const SDL_Event& e)
{
	return true;
}

bool UINode::handleMouseUp(SDL_Rect container, const SDL_Event& e)
{
	auto offset = getDrawOffset(container);

	SDL_Rect transformedDim = {
		offset.x,
		offset.y,
		calcX(style.width),
		calcY(style.height)
	};

	SDL_Point mouse = {
		e.button.x,
		e.button.y
	};

	if (SDL_PointInRect(&mouse, &transformedDim))
	{
		bool bubble = true;
		for (const auto& c : children)
		{
			bubble &= c->handleMouseUp({ transformedDim.x, transformedDim.y - scroll, transformedDim.w, transformedDim.h }, e);
		}

		if (bubble)
		{
			bubble &= onMouseUp(e);
		}

		return bubble;
	}
	return true;
}

bool UINode::onMouseUp(const SDL_Event& e)
{
	return true;
}

bool UINode::handleMouseScroll(SDL_Rect container, const SDL_Event& e)
{
	auto offset = getDrawOffset(container);

	SDL_Rect transformedDim = {
		offset.x,
		offset.y,
		calcX(style.width),
		calcY(style.height)
	};

	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_Point mouse = {
		x,
		y
	};

	if (SDL_PointInRect(&mouse, &transformedDim))
	{
		bool bubble = true;
		for (const auto& c : children)
		{
			bubble &= c->handleMouseScroll(transformedDim, e);
		}

		if (bubble && style.scrollable)
		{
			if (e.wheel.y > 0 && scroll > 0)
			{
				scroll -= 20;
				if (scroll < 0)
					scroll = 0;
			}
			else if(e.wheel.y < 0)
			{
				scroll += 20;
			}
			return false;
		}
		return bubble;
	}
	return true;
}

bool UINode::handleKeyDown(const SDL_Event& e)
{
	bool bubble = true;
	for (const auto& c : children)
	{
		bubble &= c->handleKeyDown(e);
	}

	if (bubble)
	{
		bubble &= onKeyDown(e);
	}

	return bubble;
}

bool UINode::onKeyDown(const SDL_Event& e)
{
	return true;
}

bool UINode::handleKeyUp(const SDL_Event& e)
{
	bool bubble = true;
	for (const auto& c : children)
	{
		bubble &= c->handleKeyUp(e);
	}

	if (bubble)
	{
		bubble &= onKeyUp(e);
	}

	return bubble;
}

bool UINode::onKeyUp(const SDL_Event& e)
{
	return true;
}

SDL_Point UINode::getDrawOffset(const SDL_Rect& container)
{
	SDL_Point offset = { 0 };
	// x
	if (style.xAnchor == StyleDirection::RIGHT)
	{
		offset.x = container.x + (container.w - (calcX(style.width) + style.x));
	}
	else
	{
		offset.x = container.x + style.x;
	}

	// y
	if (style.yAnchor == StyleDirection::BOTTOM)
	{
		offset.y = container.y + (container.h - (calcX(style.height) + style.y));
	}
	else
	{
		offset.y = container.y + style.y;
	}
	return offset;
}