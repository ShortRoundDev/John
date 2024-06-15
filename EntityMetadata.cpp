#include "EntityMetadata.h"

FieldType getFieldType(std::string const& type)
{
    if (type == "string")
    {
        return FieldType::JOHN_STRING;
    }
    else if (type == "int")
    {
        return FieldType::JOHN_INT;
    }
    else if (type == "float")
    {
        return FieldType::JOHN_FLOAT;
    }
    else if (type == "bool")
    {
        return FieldType::JOHN_BOOL;
    }
    return FieldType::JOHN_UNKNOWN;
}