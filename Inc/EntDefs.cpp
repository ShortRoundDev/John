#include "stdafx.h"
#include "EntDefs.h"
IObject* MakeRedneck(EntityToken const& token);
IObject* MakePistolAmmo(EntityToken const& token);
IObject* MakeUziAmmo(EntityToken const& token);
IObject* CreateEntity(EntityToken const& token)
{
    switch (static_cast<EntDefs>(token.entityId))
    {
    case EntDefs::ID_REDNECK:
        return MakeRedneck(token);
    case EntDefs::ID_PISTOLAMMO:
        return MakePistolAmmo(token);
    case EntDefs::ID_UZIAMMO:
        return MakeUziAmmo(token);
    }
    return nullptr;
}
