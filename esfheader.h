#pragma once
#include <stdint.h>
#include "file.h"

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

enum ObjectType
{
    OBJ_ROOT = 0x8000,
    OBJ_WORLD = 0x8100,
    OBJ_ZONE = 0x3000,
    OBJ_ZONE_RESOURCE = 0x3100,
    OBJ_MATERIAL_PALETTE = 0x1110,
    OBJ_MATERIAL_PALETTE_HEADER = 0x1111, // has data
    OBJ_SURFACE_ARRAY = 0x1001,
    OBJ_SURFACE = 0x1000, // data

};

const uint8_t ESF_HEADER_MAP[5] = { sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int64_t), sizeof(int64_t) };
const uint8_t OBJECT_HEADER_MAP[4] = { sizeof(uint16_t), sizeof(int16_t), sizeof(int32_t), sizeof(int64_t) };
const uint8_t SURFACE_HEADER_MAP[5] = { sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t) };

// define the per object type parsing functions
void _process_standard(FILE* file, ObjectHeader* header) { /* create a dir, call _parse_object_header for the next header if there are still children */ }
void _process_material_palette_header(FILE* file, ObjectHeader* header) {}
void _process_surface(FILE* file, ObjectHeader* header) { }

// build a handler struct to associate a value with a function pointer
static const struct
{
    uint16_t type;
    void (*handler)(FILE* file, ObjectHeader* header);
} handlers[] =
{
    { OBJ_ROOT, _process_standard },
    { OBJ_MATERIAL_PALETTE_HEADER, _process_material_palette_header },
    { OBJ_SURFACE, _process_surface }
};

// define the total number of dispatchable handlers
#define OBJ_HANDLER_COUNT (sizeof(handlers) / sizeof(handlers[0]))

// perform the dispatch lookup
void _dispatch_handler(FILE* file, ObjectHeader* objHeader)
{
    for (size_t i = 0; i < OBJ_HANDLER_COUNT; i++)
    {
        // find the handler with the matching value
        if (handlers[i].type == objHeader->type)
        {
            // call the appropriate function based on the type
            handlers[i].handler(file, objHeader);
            return;
        }
    }
}

void _parse_object_header(FILE* file, long offset)
{
    // TODO: probably do not want to fseek here
    fseek(file, offset, SEEK_SET);
    ObjectHeader h;
    sread_bytes(file, h);
    REVERSE_STRUCT_BYTES(h, OBJECT_HEADER_MAP);

    _dispatch_handler(file, &h);
    

    h.type;
    // TODO: if type is "directory like"
    if (h.type == 0x0000)
    {
        // TODO:
        CREATE_DIR("name based on h.type");
        CHANGE_CWD("name based on h.type");
        for (int32_t i = 0; i < h.childCount; i++)
        {

            _parse_object_header(file, offset + sizeof(h));
        }
    }
    else if (h.type == OBJ_MATERIAL_PALETTE_HEADER)
    {
        // else type is "file like"
        h.type;
        //use appropriate parser for the type

    }

    // return up on directory
    // TODO: if check
    CHANGE_CWD("../");
    
}

void mock(FILE* file)
{
    ESFHeader h;
    sread_bytes(file, h);
    REVERSE_STRUCT_BYTES(h, ESF_HEADER_MAP);


    if (h.magic != ESF_MAGIC)
    {
        // invalid magic
    }

    // store cwd
    char cwd[MAX_PATH_LENGTH] = { 0 };
    if (GET_CURRENT_DIR(cwd) == NULL)
    {
        //TODO: failed to get cwd
    }

    //create root folder relative to where we run from
    CREATE_DIR("root");

    if(!CHANGE_CWD("root")) {return; /*TODO*/};

    for (size_t i = 0; i < h.objectCount; i++)
    {
        /* code */
        //parse objects
        _parse_object_header(file, h.offset);
    }
    

}