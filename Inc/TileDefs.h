#pragma once

#include "JohnMapFile.h"

enum class TileDefs : uint16_t
{
    ID_BRICKS = 1,
    ID_GRASS1 = 2,
    ID_GRASS2 = 3,
    ID_BONGO1 = 4,
    ID_BONGO2 = 5,
    ID_CONCRETE = 6,
};

class IBrush;
IBrush* CreateWall(WallToken const& token, int x, int y, Texture const* texture);
IBrush* CreateFloor(WallToken const& token, int x, int y, Texture const* texture);
IBrush* CreateCeiling(WallToken const& token, int x, int y, Texture const* texture);

