#include "stdafx.h"
#include "EntDefs.h"
IObject* MakeRedneck(EntityToken const& token);
IObject* CreateEntity(EntityToken const& token)
{
    switch (static_cast<EntDefs>(token.entityId))
    {
    case EntDefs::ID_REDNECK:
        return MakeRedneck(token);
    }
    return nullptr;
}
