#pragma once

#include "JohnMapFile.h"

enum class EntDefs : uint16_t
{
    ID_REDNECK = 1000,
    ID_PISTOLAMMO = 1002,
    ID_UZIAMMO = 1001,
};

class IObject;


IObject* CreateEntity(EntityToken const& token);
