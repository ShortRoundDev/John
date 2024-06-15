#include "TileArea.h"

#include <cstdlib>
#include <Windows.h>

#include "App.h"
#include "Tile.h"

TileArea::TileArea() : UINode(
	{
		16, 80,
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
	loadTiles();
}

TileArea::~TileArea()
{

}

void TileArea::loadTiles()
{
	static char buffer[128];
	FILE* fp = fopen("Resources/tiles.conf", "r");
	if (fp == NULL)
	{
		MessageBoxA(NULL, (std::string("Tiles.conf: ") + strerror(errno)).c_str(), NULL, MB_OK);
		return;
	}
	int i = 0;

	while (fgets(buffer, 127, fp))
	{
		buffer[127] = 0;
		std::string line = buffer;

		EntityMetadata metadata;
		Texture* texture = nullptr;
		if (APP->tryLoadEntityConfig("Resources/" + line + ".conf", metadata) && APP->tryLoadTexture(metadata.texture, line, &texture))
		{
			APP->idTextureMapping[metadata.id] = line;
			APP->entityMetadata[metadata.id] = metadata.fields;
			auto tile = new Tile(i++, metadata.id, texture);
			addChild(tile);
		}
	}
	//fseek(fp, 0, SEEK_END);
	//size_t fileSize = ftell(fp);
	//fseek(fp, 0, SEEK_SET);
	//char* text = (char*)calloc(fileSize, sizeof(char) + 1);
	//fread(text, sizeof(char), fileSize, fp);
	//fclose(fp);

	//// parse
	//char* c = text;
	//int i = 0;
	//while (*c != 0)
	//{
	//	char* start = c;
	//	for (; *c != '\n' && *c != 0; c++);
	//	if (*c == '\n')
	//	{
	//		*c = 0;
	//		c++;
	//	}

	//	char* next;
	//	int tileNum = strtol(start, &next, 10);
	//	if (next == start || *next != 0 || errno == ERANGE)
	//	{
	//		MessageBoxA(NULL, ("Couldn't parse tile.conf [" + std::string(start) + "]").c_str(), NULL, MB_OK);
	//		continue;
	//	}

	//	Texture* t = NULL;
	//	if (!APP->tryLoadTexture(("Resources/Tiles/" + std::to_string(tileNum) + ".png"), std::to_string(tileNum), &t))
	//	{
	//		MessageBoxA(NULL, ("Couldn't load tile " + std::to_string(tileNum)).c_str(), NULL, MB_OK);
	//		continue;
	//	}
	//	tileTextures[tileNum] = t;
	//	APP->idTextureMapping[tileNum] = std::to_string(tileNum);

	//	// create tile button

	//	auto tile = new Tile(i++, tileNum, t);
	//	children.push_back(tile);
	//}
}