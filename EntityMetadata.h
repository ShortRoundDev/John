#pragma once

#include <string>
#include <vector>
typedef enum FieldType
{
    JOHN_STRING,
    JOHN_INT,
    JOHN_FLOAT,
    JOHN_BOOL,
    JOHN_UNKNOWN
} FieldType;

typedef struct EntityMetadataField
{
    std::string name;
    FieldType type;
} EntityMetadataField;

typedef struct EntityMetadata
{
    uint16_t id;
    std::string texture;
    std::vector<EntityMetadataField> fields;
} EntityMetadata;

FieldType getFieldType(std::string const& type);