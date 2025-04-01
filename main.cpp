#include <stdio.h>

#include "bytes.h"
#include "file.h"
#include "esfheader.h"


// typedef struct
// {
//     void* key;//uint16_t key;
//     void* value;
//     UT_hash_handle hh;
// } uint16_void_dict;

int main()
{

    hash_item* dict = NULL;

    uint16_t key = 123;
    // char* value = "my value";
    uint32_t value = 12345;

    ///// hash_item* test = (hash_item*)malloc(sizeof(hash_item));
    ///// test->key = &key;
    ///// test->value = &value;

    // DICT_ADD_ITEM(dict, key, value);
    // HASH_ADD_INT(dict, key, test);
    // HASH_ADD(hh, dict, key, sizeof(key), test);
    /////HASH_ADD_KEYPTR(hh, dict, &key, sizeof(key), test);

    // struct test_hash* item = (struct test_hash*)DICT_GET_ITEM(dict, key);
    /////hash_item* item = NULL;
    // HASH_FIND_INT(dict, &key, item);
    /////HASH_FIND(hh, dict, &key, sizeof(key), item);
    // HASH_FIND_PTR(dict, &key, item);
    // HASH_FIND(hh, dict, &key, sizeof(key), item);

    DICT_ADD_ITEM(dict, key, value);


    DICT_FOR_EACH(dict, entry)
    {
        if (entry)
        {
            printf("%u\n", *((uint32_t*)(entry->value)));
        }
    }

    printf("get item\n");
    hash_item* item = (hash_item*)DICT_GET_ITEM(dict, key);
    printf("g0t item\n");

//     hash_item* cur = NULL;
//     hash_item* tmp = NULL;

//     HASH_ITER(hh, dict, cur, tmp)
//     {
//         if(cur)
//             printf("%u\n", *((uint32_t*)(cur->value)));
//     }
// printf("ok 1");
    if (!item)
    {
        printf("fail\n");
    }
    else
    {
        printf("ok");
        printf("%u\n", *((uint32_t*)(item->value)));
    }

    DICT_FREE(dict);

    return 0;

    const char* expected_cwd = "/home/nemo/src/util";
    bool islittle = is_little_endian();

    char cwd[256] = { 0 };

    if (GET_CURRENT_DIR(cwd) != NULL)
    {
        printf("CWD: %s\n", cwd);
        if (strcmp(cwd, expected_cwd) == 0)
        {
            FileInfo fileInfo = GET_FILE_INFO("test.txt");
            
        }
    }

    // TODO: check acceptable windows formats for paths
    if(CREATE_DIR_R("../tempdir/newdir") != 0)
    {
        //TODO: can handle error codes...
        printf("fail sauce\n");
    }


    uint32_t reverse = 0xEFBEADDE;
    printf("reverse %X\n", reverse);
    sreversebytes(reverse);
    printf("reverse %X\n", reverse);

    FILE* f = fopen("test.txt", "rb");

    ESFHeader outHeader = {0};
    sread_bytes(f, outHeader);


    PRINT_VALUE_STRING(outHeader.magic);
    sreversebytes(outHeader.magic);
    PRINT_VALUE_STRING(outHeader.magic);

    PRINT_VALUE_STRING(outHeader.objectCount);
    sreversebytes(outHeader.objectCount);
    PRINT_VALUE_STRING(outHeader.objectCount);

    PRINT_VALUE_STRING(outHeader.unknown1, '\0');
    sreversebytes(outHeader.unknown1);
    PRINT_VALUE_STRING(outHeader.unknown1);
    
    PRINT_VALUE_STRING(outHeader, '\n');

    fclose(f);

    uint8_t flags = 0;
    printf("flags \n");

    PRINT_BINARY_STRING(flags, '\n');
    sset_flag(flags, 5);
    PRINT_BINARY_STRING(flags, '\n');
    stoggle_flag(flags, 5);
    PRINT_BINARY_STRING(flags, '\n');
    stoggle_flag(flags, 5);
    PRINT_BINARY_STRING(flags, '\n');

    uint16_t rotate = 1;
    printf("rot %u\n", rotate);
    srot_left(rotate, 1);
    printf("rot %u\n", rotate);
    srot_right(rotate, 2);
    printf("rot %u\n", rotate);

    printf("\n");

    return EXIT_SUCCESS;
}