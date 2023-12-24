#pragma once
#include "UINode.h"

#include <memory>

#define PROPS (PropertiesEditor::instance)

class PropertiesEditor : public UINode
{
public:

	static std::unique_ptr<PropertiesEditor> instance;
	static PropertiesEditor* init();

	PropertiesEditor();
	~PropertiesEditor();

	void draw(const SDL_Rect& container);
	void clear();
	void addBoolean(std::string name, bool* value);
	void addText(std::string name, std::string* value);
	void addNumber(std::string name, uint8_t* value);
};

