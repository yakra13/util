#include "bytes.h"
#include "file.h"

#include <stdio.h>

#pragma pack(push, 1)
typedef struct MockHeader
{
    uint16_t field1;
    uint32_t field2;
    uint8_t field3;
};
#pragma pack(pop)


int main()
{
    char cwd[256] = { 0 };
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("CWD: %s\n", cwd);
    }

    if(CREATE_DIR_R("../tempdir/newdir") != 0)
    {
        printf("fail sauce\n");
    }
    

    bool islittle = is_little_endian();

    uint32_t reverse = 0xEFBEADDE;
    printf("reverse %X\n", reverse);
    sreversebytes(reverse);
    printf("reverse %X\n", reverse);

    FILE* f = fopen("test.txt", "rb");

    MockHeader outHeader = {0};
    sread_bytes(f, outHeader);

    PRINT_VALUE_STRING(outHeader.field1);
    sreversebytes(outHeader.field1);
    PRINT_VALUE_STRING(outHeader.field1);

    PRINT_VALUE_STRING(outHeader.field2);
    sreversebytes(outHeader.field2);
    PRINT_VALUE_STRING(outHeader.field2);

    PRINT_VALUE_STRING(outHeader.field3, 0);
    sreversebytes(outHeader.field3);
    PRINT_VALUE_STRING(outHeader.field3);
    
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