#pragma once

#include "JohnMapFile.h"

enum class EntDefs : uint16_t
{
    ID_REDNECK = 1000,
};

class IObject;


IObject* CreateEntity(EntityToken const& token);
