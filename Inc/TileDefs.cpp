#include "stdafx.h"
#include "TileDefs.h"
IBrush* MakeSimpleWall(WallToken const& token);
IBrush* MakeSimpleFloor(WallToken const& token);
IBrush* MakeSimpleCeiling(WallToken const& token);
IBrush* CreateWall(WallToken const& token)
{
    switch (static_cast<TileDefs>(token.wallType))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
        return MakeSimpleWall(token);
    }
    return nullptr;
}

IBrush* CreateFloor(WallToken const& token)
{
    switch (static_cast<TileDefs>(token.floor))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
        return MakeSimpleFloor(token);
    }
    return nullptr;
}

IBrush* CreateCeiling(WallToken const& token)
{
    switch (static_cast<TileDefs>(token.ceiling))
    {
    case TileDefs::ID_BRICKS:
    case TileDefs::ID_GRASS1:
    case TileDefs::ID_GRASS2:
    case TileDefs::ID_BONGO1:
    case TileDefs::ID_BONGO2:
    case TileDefs::ID_CONCRETE:
        return MakeSimpleCeiling(token);
    }
    return nullptr;
}
