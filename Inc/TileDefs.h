#pragma once

#include "JohnMapFile.h"

enum class TileDefs : uint16_t
{
    ID_BRICKS,
    ID_GRASS1,
    ID_GRASS2,
    ID_BONGO1,
    ID_BONGO2,
    ID_CONCRETE,
};

class IBrush;
IBrush* CreateWall(WallToken const& token);
IBrush* CreateFloor(WallToken const& token);
IBrush* CreateCeiling(WallToken const& token);

