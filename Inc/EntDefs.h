#pragma once

#include "JohnMapFile.h"

enum class EntDefs : uint16_t
{
    ID_REDNECK = 1000,
    ID_PISTOLAMMO = 1002,
    ID_UZIAMMO = 1001,
    ID_EMPLOYEE = 1003,
    ID_HOBO = 1005,
    ID_BARREL = 5006,
};

class IObject;


IObject* CreateEntity(EntityToken const& token);
