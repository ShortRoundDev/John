#include "PropertiesEditor.h"

#include "App.h"

#include "PropertyLine.h"
#include "Checkbox.h"
#include "NumberInput.h"
#include "TextButton.h"
#include "FloatInput.h"

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
		deleteChild(children[i]);
	}
}

void PropertiesEditor::addBoolean(std::string name, bool* value)
{
	addChild(
		new PropertyLine(
			(uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
			new Checkbox(value)
		)
	);
}

void PropertiesEditor::addBoolean(std::string name, bool initialValue, std::function<void(bool)> onClick)
{
	addChild(
		new PropertyLine(
			(uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
			new Checkbox(initialValue, onClick)
		)
	);
}

void PropertiesEditor::addText(std::string name, std::string* value)
{
	addChild(
		new PropertyLine(
			(uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
			new TextButton(value)
		)
	);
}

void PropertiesEditor::addText(std::string name, std::string initialValue, std::function<void(std::string)> onSave)
{
	addChild(
		new PropertyLine(
			(uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
			new TextButton(initialValue, onSave)
		)
	);
}


void PropertiesEditor::addNumber(std::string name, uint8_t* value)
{
	addChild(
		new PropertyLine(
			(uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
			new NumberInput(value)
		)
	);
}

void PropertiesEditor::addFloat(std::string name, float initialValue, std::function<void(float)> onChange)
{
	addChild(
		new PropertyLine(
            (uint16_t)children.size() + toAdd.size() - toRemove.size(), name,
            new FloatInput(initialValue, onChange)
        )
    );
}