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
	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* text = (char*)calloc(fileSize, sizeof(char) + 1);
	fread(text, sizeof(char), fileSize, fp);
	fclose(fp);

	char* c = text;
	int i = 0;
	while (*c != 0)
	{
		char* key = c;
		// find =
		for (; *c != '=' && *c != '\n' && *c != 0; c++);
		if (*c == '\n')
		{
			// fucked up line. no =. Skip line. Maybe warn?
			c++;
			continue;
		}
		else if (*c == 0)
		{
			// fucked up line. no =. End of file. break loop. Maybe warn?
			break;
		}
		else
		{
			*c = 0;
			c++;
		}
		char* value = c;

		//find end of line or end of file
		for (; *c != '\n' && *c != 0; c++);
		if (*c == '\n')
		{
			*c = 0;
			c++;
		}

		int id = atoi(value);

		Texture* t = NULL;
		if (!APP->tryLoadTexture(std::string("Resources/Entities/") + key + ".png", key, &t))
		{
			MessageBoxA(NULL, ("Failed to load entity thumb for " + std::string(key)).c_str(), NULL, MB_OK);
			continue;
		}
		APP->idTextureMapping[id] = key;

		children.push_back(new EntityButton(i++, id, t, std::string(key)));

	}
}