#include <stdio.h>

#include "bytes.h"
#include "file.h"
#include "esfheader.h"


int main()
{
#ifdef DEBUG
    const char* expected_cwd = "/home/nemo/src/util";
    bool islittle = is_le_c();

    char cwd[256] = { 0 };

    if (GET_CURRENT_DIR(cwd) != NULL)
    {
        printf("CWD: %s\n", cwd);
        if (strcmp(cwd, expected_cwd) == 0)
        {
            //FileInfo fileInfo = GET_FILE_INFO("test.txt");
            //TODO
        }
    }

    // TODO: check acceptable windows formats for paths
    if(CREATE_DIR_R("../tempdir /newdir") != 0)
    {
        //TODO: can handle error codes...
        printf("fail sauce\n");
    }

    uint32_t reverse = 0xEFBEADDE;
    printf("reverse %X\n", reverse);
    REVERSE_BYTES(reverse);
    printf("reverse %X\n", reverse);

    FILE* f = fopen("test.txt", "rb");

    ESFHeader outHeader = {0};
    sread_bytes(f, outHeader);


    PRINT_VALUE_STRING(outHeader.magic);
    REVERSE_BYTES(outHeader.magic);
    PRINT_VALUE_STRING(outHeader.magic);

    PRINT_VALUE_STRING(outHeader.objectCount);
    REVERSE_BYTES(outHeader.objectCount);
    PRINT_VALUE_STRING(outHeader.objectCount);

    PRINT_VALUE_STRING(outHeader.unknown1, '\0');
    REVERSE_BYTES(outHeader.unknown1);
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
#else
    FILE* tunaria;
    tunaria = fopen("C:\\Users\\goku1\\source\\TUNARIA.ESF", "rb");
    if (!tunaria)
    {

    }

    parse(tunaria);

    fclose(tunaria);
#endif

    return EXIT_SUCCESS;
}