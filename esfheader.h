#pragma once
#include <stdint.h>
#include "file.h"
// #include "uthash.h"
#include "dictionary.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int32_t ESF_MAGIC = 0x464A424F;

#pragma pack(push, 1)
    typedef struct 
    {
        int32_t magic;
        int32_t objectCount;
        int32_t fileType;
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

typedef struct 
{
    int id;//uint16_t id;
    const char* name;
    size_t occurrences;
    void (*processor)(FILE* file, ObjectHeader* header);
} ObjData;


typedef struct 
{
    uint16_t key;
    ObjData* value;
    //void (*value)(FILE* file, ObjectHeader* header);
    UT_hash_handle hh;
} ObjTypeMap;

typedef void (*ObjProcessor)(FILE*, ObjectHeader*);

// ObjTypeMap* typeHandlers = NULL;
// Dictionary* dict = DICT_NEW(uint16_t, ObjectHeader);

hash_item* processor_lookup = NULL;

// uint16_t OBJ_ROOT = 0x8000;
// uint16_t OBJ_WORLD = 0x8100;
// uint16_t OBJ_ZONE = 0x3000;
// uint16_t OBJ_ZONE_RESC = 0x3100;
// uint16_t OBJ_MATR_PALT = 0x1110;
// uint16_t OBJ_MATR_PALT_H = 0x1111; // has data
// uint16_t OBJ_SURFACE_ARR = 0x1001;
// uint16_t OBJ_SURFACE = 0x1000; // data
int OBJ_ROOT = 0x8000;
int OBJ_WORLD = 0x8100;
int OBJ_ZONE = 0x3000;
int OBJ_ZONE_RESC = 0x3100;
int OBJ_MATR_PALT = 0x1110;
int OBJ_MATR_PALT_H = 0x1111; // has data
int OBJ_SURFACE_ARR = 0x1001;
int OBJ_SURFACE = 0x1000; // data


const uint8_t ESF_HEADER_MAP[] = { sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int64_t), sizeof(int64_t) };
const uint8_t OBJECT_HEADER_MAP[] = { sizeof(uint16_t), sizeof(int16_t), sizeof(int32_t), sizeof(int64_t) };
const uint8_t SURFACE_HEADER_MAP[] = { sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t), sizeof(int32_t) };

// define the per object type parsing functions
// void _process_standard(FILE* file, ObjectHeader* header) { /* create a dir, call _parse_object_header for the next header if there are still children */ }
// void _process_material_palette_header(FILE* file, ObjectHeader* header) {}



void _process_standard(FILE* file, ObjectHeader* header)
{
    // Basic folder like object
    ObjData* objd = (ObjData*)DICT_GET_ITEM(processor_lookup, header->type);

    CREATE_DIR(objd->name);

    CHANGE_CWD(objd->name);
}

void _process_material_palette_header(FILE* file, ObjectHeader* header)
{
    ObjData* objd = (ObjData*)DICT_GET_ITEM(processor_lookup, header->type);

    uint32_t id = 0;
    sread_le(file, id);
    // sread_bytes(file, id);
    // REVERSE_BYTES(id);

    char file_name[100] = { 0 };
    int temp = 1234; //TODO: track an actual count of occurrences; possibly in the ObjData struct?
    sprintf(file_name, "%s_%zu-data.txt", objd->name, objd->occurrences);
    //TODO checks
    FILE* out = fopen(file_name, "w");
    if (!out)
    {
        //TODO:
    }

    fprintf(out, "0x%X %u", id, id);

    fclose(out);

    objd->occurrences++;
}

void _process_surface(FILE* file, ObjectHeader* header)
{
    ObjData* objd = (ObjData*)DICT_GET_ITEM(processor_lookup, header->type);

    SurfaceHeader h = { 0 };
    ColorPalette palette = { 0 };
    
    sread_bytes(file, h);
    REVERSE_STRUCT_BYTES(h, SURFACE_HEADER_MAP);

    // TODO: place mip maps in a folder?

    if (h.depth > 2)
    {
        sread_le(file, palette.count);
        palette.colors = (Color*)malloc(sizeof(Color) * palette.count);
        //TODO: malloc checks

        for (int32_t i = 0; i < palette.count; i++)
        {
            sread_bytes(file, palette.colors[i]);
        }

        if (h.mip > 0)
        {
            int32_t mipLevel = 0;
            while (mipLevel != 1)
            {
                sread_bytes(file, mipLevel);
                //TODO: mip level should be a count down
                int32_t totalMipBytes = mipLevel * mipLevel;

                // TODO: is it always 4 bytes per color?
                uint8_t* image = (uint8_t*)malloc(totalMipBytes * 4);
                uint8_t* img_ptr = image;
                //TODO: malloc checks
                
                for (int32_t i = 0; i < totalMipBytes; i++)
                {
                    uint8_t paletteIndex = 0;
                    sread_bytes(file, paletteIndex);

                    *img_ptr++ = palette.colors[paletteIndex].r;
                    *img_ptr++ = palette.colors[paletteIndex].g;
                    *img_ptr++ = palette.colors[paletteIndex].b;
                    *img_ptr++ = palette.colors[paletteIndex].a;
                }

                char file_name[100] = { 0 };
                int temp = 1234; //TODO: track an actual count of occurrences; possibly in the ObjData struct?
                sprintf(file_name, "%s_%zu-%d.png", objd->name, objd->occurrences, mipLevel);
                
                stbi_write_png(file_name, mipLevel, mipLevel, 4, image, mipLevel * 4);

                free(image);
            }
        }
        else
        {
            // TODO:
        }
        
    }

    objd->occurrences++;
}

// dict->Add(dict, NULL, NULL);

// void add_item(ObjTypeMap** dict, uint16_t key, void (*value)(FILE*, ObjectHeader*))
// {
//     ObjTypeMap* s;

//     HASH_FIND_INT(*dict, &key, s);
//     if (s == NULL)
//     {
//         s = (ObjTypeMap*)malloc(sizeof(ObjTypeMap));
//         s->key = key;
//         HASH_ADD_INT(*dict, key, s);
//     }
//     // Add the key if it doesnt exist, if it does just modify its value
//     s->value = value;

// }

// build a handler struct to associate a value with a function pointer
// static const struct
// {
//     uint16_t type;
//     void (*handler)(FILE* file, ObjectHeader* header);
// } handlers[] =
// {
//     { OBJ_ROOT, _process_standard },
//     { OBJ_MATERIAL_PALETTE_HEADER, _process_material_palette_header },
//     { OBJ_SURFACE, _process_surface }
// };

// define the total number of dispatchable handlers
// #define OBJ_HANDLER_COUNT (sizeof(handlers) / sizeof(handlers[0]))

// perform the dispatch lookup
// void _dispatch_handler(FILE* file, ObjectHeader* objHeader)
// {
//     // ObjProcessor f = (ObjProcessor)DICT_GET_ITEM(dict->dict, objHeader->type);
//     // f(file, objHeader);

//     // for (size_t i = 0; i < OBJ_HANDLER_COUNT; i++)
//     // {
//     //     // find the handler with the matching value
//     //     if (handlers[i].type == objHeader->type)
//     //     {
//     //         // call the appropriate function based on the type
//     //         handlers[i].handler(file, objHeader);
//     //         return;
//     //     }
//     // }
// }

void _parse_object_header(FILE* file)//, long offset)
{
    // TODO: probably do not want to fseek here
    //fseek(file, offset, SEEK_SET);
    ObjectHeader h;
    sread_bytes(file, h);
    REVERSE_STRUCT_BYTES(h, OBJECT_HEADER_MAP);

    // _dispatch_handler(file, &h);
    // ObjProcessor f = ((ObjProcessor)DICT_GET_ITEM(dict_obj_type_to_processor, h.type))(file, &h);
    // f(file, &h);

    // Run the processor function for the object type
    ObjData* obj = (ObjData*)DICT_GET_ITEM(processor_lookup, h.type);
    if (!obj)
    {
        // This obj header has not been added to the dictionary
        printf("Object type not implemented: %X\nAt file index: %X\n", h.type, (unsigned int)(ftell(file) - sizeof(ObjectHeader)));
        printf("MANUALLY CLEAN THE OUTPUT DIRECTORY BEFORE RUNNING AGAIN\n");
        // TODO: change directory back to original etc
        fclose(file);
        exit(1);
    }

    obj->processor(file, &h);

    for (size_t i = 0; i < h.childCount; i++)
    {
        _parse_object_header(file);//, offset + sizeof(ObjectHeader));
    }
    
    // h.type;
    // // TODO: if type is "directory like"
    // if (h.type == 0x0000)
    // {
    //     // TODO:
    //     CREATE_DIR("name based on h.type");
    //     CHANGE_CWD("name based on h.type");
    //     for (int32_t i = 0; i < h.childCount; i++)
    //     {

    //         _parse_object_header(file, offset + sizeof(h));
    //     }
    // }
    // else if (h.type == OBJ_MATERIAL_PALETTE_HEADER)
    // {
    //     // else type is "file like"
    //     h.type;
    //     //use appropriate parser for the type

    // }

    // return up on directory
    // TODO: if check
    CHANGE_CWD("../");
    
}

void parse(FILE* file)
{
    
    // dict->Add(dict, (void*)&OBJ_ROOT, _process_standard);
    // dict->Add(dict, (void*)&OBJ_MATERIAL_PALETTE_HEADER, _process_material_palette_header);
    // dict->Add(dict, (void*)&OBJ_SURFACE, _process_surface);

    // ObjData* od_root = (ObjData*)malloc(sizeof(ObjData));
    // od_root->name = "Root";
    // od_root->processor = _process_standard;

    ObjData od_arr[] = 
    {
        { OBJ_ROOT, "Root", 0, _process_standard },
        { OBJ_WORLD, "World", 0, _process_standard },
        { OBJ_ZONE, "Zone", 0, _process_standard },
        { OBJ_ZONE_RESC, "Zone Resource", 0, _process_standard },
        { OBJ_MATR_PALT, "Material Palette", 0, _process_standard },
        { OBJ_MATR_PALT_H, "Material Palette Header", 0, _process_material_palette_header },
        { OBJ_SURFACE_ARR, "Surface Array", 0, _process_standard },
        { OBJ_SURFACE, "Surface", 0, _process_surface },
    };

    for (size_t i = 0; i < sizeof(od_arr) / sizeof(od_arr[0]); i++)
    {
        DICT_ADD_ITEM(processor_lookup, od_arr[i].id, od_arr[i]);
    }

    hash_item* item = NULL;
    // HASH_FIND(hh, processor_lookup, &OBJ_ROOT, sizeof(uint16_t), item);
    HASH_FIND_INT(processor_lookup, &OBJ_ROOT, item);

    printf("Found Key: %X\n", *(uint16_t*)(item->key));
    //DICT_DEL_ITEM(processor_lookup, OBJ_ROOT);

    hash_item* cur = NULL;
    hash_item* tmp = NULL;

    HASH_ITER(hh, processor_lookup, cur, tmp)
    {
        printf("Key: %X : ", *(uint16_t*)(cur->key));
        printf("Value: %p\n", cur->value);
    }

    // Setup dispatcher dictionary to link object type with appropriate processor function
    // DICT_ADD_ITEM(processor_lookup, OBJ_ROOT, od_arr[0]);
    // DICT_ADD_ITEM(processor_lookup, OBJ_WORLD, _process_standard);
    // DICT_ADD_ITEM(processor_lookup, OBJ_ZONE, _process_standard);
    // DICT_ADD_ITEM(processor_lookup, OBJ_ZONE_RESOURCE, _process_standard);
    // DICT_ADD_ITEM(processor_lookup, OBJ_MATERIAL_PALETTE, _process_standard);
    // DICT_ADD_ITEM(processor_lookup, OBJ_MATERIAL_PALETTE_HEADER, _process_material_palette_header);
    // DICT_ADD_ITEM(processor_lookup, OBJ_SURFACE_ARRAY, _process_standard);
    // DICT_ADD_ITEM(processor_lookup, OBJ_SURFACE, _process_surface);

    // void (*myfunc)(FILE*, ObjectHeader*) = (void (*)(FILE*, ObjectHeader*))DICT_GET_ITEM(dict->dict, OBJ_ROOT);
    // myfunc(f, o);

    ESFHeader h;
    sread_bytes(file, h);
    REVERSE_STRUCT_BYTES(h, ESF_HEADER_MAP);


    if (h.magic != ESF_MAGIC)
    {
        // TODO: invalid magic
        return;
    }

    // store cwd
    char cwd[MAX_PATH_LENGTH] = { 0 };
    if (GET_CURRENT_DIR(cwd) == NULL)
    {
        //TODO: failed to get cwd
    }

    //create root folder relative to where we run from
    CREATE_DIR("parsed_esf");

    if(!CHANGE_CWD("parsed_esf")) {return; /*TODO*/};

    // This first iteration should always be 1 item: root
    for (size_t i = 0; i < h.objectCount; i++)
    {
        /* code */
        //parse objects
        _parse_object_header(file);//, h.offset);
    }
    

    DICT_FREE(*processor_lookup);
}