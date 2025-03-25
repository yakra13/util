#pragma once
#include <stdint.h>

#define ESF_MAGIC 0x4F424A46;

#pragma pack(push, 1)
typedef struct 
{
    int32_t magic;
    int32_t objectCount;
    int32_t unknown1;
    int64_t offset;
    int64_t unknown2;
} ESFHeader;

typedef struct
{
    uint16_t type;
    int16_t version;
    int32_t length;
    int32_t childCount;
} ObjectHeader;

typedef struct
{
    int32_t id;
    int32_t width;
    int32_t height;
    int32_t depth;
    int32_t mip;
} SurfaceHeader;

typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;


typedef struct
{
    int32_t count;
    Color* colors;
} ColorPalette;


#pragma pack(pop)