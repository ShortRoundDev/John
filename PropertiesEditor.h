#pragma once
#include "UINode.h"

#include <memory>
#include <functional>

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
	void addBoolean(std::string name, bool initialValue, std::function<void(bool)> onClick);

	void addText(std::string name, std::string* value);
	void addText(std::string name, std::string initialValue, std::function<void(std::string)> onSave);

	void addNumber(std::string name, uint8_t* value);

	void addFloat(std::string name, float* value);
	void addFloat(std::string name, float initialValue, std::function<void(float)> onChange);
};

