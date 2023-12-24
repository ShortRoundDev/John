#include "PropertiesEditor.h"

#include "App.h"

#include "PropertyLine.h"
#include "Checkbox.h"
#include "NumberInput.h"
#include "TextButton.h"

std::unique_ptr<PropertiesEditor> PropertiesEditor::instance;
PropertiesEditor* PropertiesEditor::init()
{
	instance = std::make_unique<PropertiesEditor>();
	return instance.get();
}

PropertiesEditor::PropertiesEditor() : UINode(
	{
		16, 688 + 32,
		224, 256,
		"",
		CGA_LT_BLUE,
		StyleDirection::TOP,
		StyleDirection::LEFT,
		CGA_LT_CYAN,
		false, true
	}
)
{
}

PropertiesEditor::~PropertiesEditor()
{
	UINode::~UINode();
}

void PropertiesEditor::draw(const SDL_Rect& container)
{
	UINode::draw(container);
}

void PropertiesEditor::clear()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
		children[i] = nullptr;
	}
	children.clear();
}

void PropertiesEditor::addBoolean(std::string name, bool* value)
{
	children.push_back(
		new PropertyLine(
			(uint16_t)children.size(), name,
			new Checkbox(value)
		)
	);
}

void PropertiesEditor::addText(std::string name, std::string* value)
{
	children.push_back(
		new PropertyLine(
			(uint16_t)children.size(), name,
			new TextButton(value)
		)
	);
}

void PropertiesEditor::addNumber(std::string name, uint8_t* value)
{
	children.push_back(
		new PropertyLine(
			(uint16_t)children.size(), name,
			new NumberInput(value)
		)
	);
}