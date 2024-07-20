#include "stdafx.h"
#include "TileDefs.h"
IBrush* MakeSimpleWall(WallToken const& token, int x, int y, Texture const* texture);
IBrush* MakeSimpleFloor(WallToken const& token, int x, int y, Texture const* texture);
IBrush* MakeSimpleCeiling(WallToken const& token, int x, int y, Texture const* texture);
IBrush* CreateWall(WallToken const& token, int x, int y, Texture const* texture)
{
    switch (static_cast<TileDefs>(token.wallType))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
    default:
        return MakeSimpleWall(token, x, y, texture);
    }
    return nullptr;
}

IBrush* CreateFloor(WallToken const& token, int x, int y, Texture const* texture)
{
    switch (static_cast<TileDefs>(token.floor))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
    default:
        return MakeSimpleFloor(token, x, y, texture);
    }
    return nullptr;
}

IBrush* CreateCeiling(WallToken const& token, int x, int y, Texture const* texture)
{
    switch (static_cast<TileDefs>(token.ceiling))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
    default:
        return MakeSimpleCeiling(token, x, y, texture);
    }
    return nullptr;
}
