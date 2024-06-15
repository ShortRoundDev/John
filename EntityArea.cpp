#include "EntityArea.h"

#include "Windows.h"

#include "App.h"
#include "EntityButton.h"

EntityArea::EntityArea() : UINode(
	{
		16, 256 + 144,
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
	loadEntities();
}

void EntityArea::loadEntities()
{
	FILE* fp = fopen("Resources/entities.conf", "r");
	if (fp == NULL)
	{
		MessageBoxA(NULL, (std::string("Entities.conf: ") + strerror(errno)).c_str(), NULL, MB_OK);
		return;
	}

	static char buffer[128];

	int i = 0;
	while (fgets(buffer, 127, fp))
	{
		buffer[127] = 0;
		std::string line = buffer;
		EntityMetadata metadata;
		Texture* texture = nullptr;
		if (line[line.length() - 1] == '\n')
		{
			line = line.substr(0, line.length() - 1);
		}
		if (APP->tryLoadEntityConfig("Resources/Entities/" + line + ".conf", metadata) && APP->tryLoadTexture(metadata.texture, line, &texture))
		{
			APP->idTextureMapping[metadata.id] = line;
			APP->entityMetadata[metadata.id] = metadata.fields;
			auto button = new EntityButton(i++, metadata.id, texture, line);
			addChild(button);
		}
	}
}