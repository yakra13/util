#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "dictionary.h"
#include "file.h"
#include "bytes.h"

// typedef int KEY_TYPE;
// typedef float VALUE_TYPE;


// #define CAST_AS_KEY(k) *(KEY_TYPE*)k
// #define CAST_AS_VALUE(v) *(VALUE_TYPE*)v

void dict_example()
{
    // Dictionary* d = DICT_NEW_DICT(uint16_t, char*);

    // uint16_t key = 1;
    // char* value = "something here";
    // char* value2 = "another thing";
    // char* value3 = "i dunno";

    // DICT_ADD_ENTRY(d, key, value)
    // key = 2;
    // DICT_ADD_ENTRY(d, key, value2)

    // BEGIN_DICT_ITER(d, entry)
    //     printf("key: %hu, value: %s\n", *(uint16_t*)(entry->key), *(const char**)entry->value);
    // END_DICT_ITER()

    // key = 1;
    // DICT_SET_VALUE(d, key, value3)

    // BEGIN_DICT_ITER(d, entry)
    //     printf("key: %hu, value: %s\n", *(uint16_t*)(entry->key), *(const char**)entry->value);
    // END_DICT_ITER()

    // DICT_FREE(d)
}

void entropy_example()
{
    double e = calculate_entropy("main");
    printf("entropy of %.3f\n", e);
}

void directory_example()
{
    // TODO
    // CREATE_DIR("")
    // CREATE_DIR_R("")
    // CHANGE_CWD("")

    // char* buffer[MAX_PATH_LENGTH] = {0};
    // GET_CURRENT_DIR(buffer)


}

void bytes_example()
{
    uint8_t flags = 0;
    uint8_t mask = 0xF0;

    char bit_str_buffer[64] = { 0 };

    SET_BIT_FLAG(flags, 0)

    SET_BIT_FLAG(flags, 2)

    SET_BIT_FLAG(flags, 4)

    GET_BIT_STRING(flags, bit_str_buffer);

    printf("bits: %s\n", bit_str_buffer);
}

int main(int argc, char** argv)
{

    uint8_t ui8 = 123;
    uint16_t ui16 = 112;
    uint32_t ui32 = 12345;
    uint64_t ui64 = 1234567890;
    int16_t i16 = -450;

    printf("%" PRIu8 " has %d digits\n", ui8, GET_UINT_DIGIT_COUNT(ui8));
    printf("%" PRIu16 " has %d digits\n", ui16, GET_UINT_DIGIT_COUNT(ui16));
    printf("%" PRIu32 " has %d digits\n", ui32, GET_UINT_DIGIT_COUNT(ui32));
    printf("%" PRIu64 " has %d digits\n", ui64, GET_UINT_DIGIT_COUNT(ui64));

    printf("%" PRId16 " has %d digits\n", i16, GET_SINT_DIGIT_COUNT(i16));

    char buf[25] = { 0 };
    UINT_TO_ASCII(ui64, buf);
    printf("%s\n", buf);
    UINT_TO_ASCII(ui16, buf);
    printf("%s\n", buf);
    // TODO: i broke this...
    SINT_TO_ASCII(i16, buf);
    printf("%s\n", buf);

    uint32_t converted = *(uint16_t*)(CONVERT_SINT_TO_UINT(i16));
    printf("converted %" PRIu32 " \n", converted);

    printf("do shifting\n");

    uint16_t shiftme = 1;
    uint16_t shiftcheck = 1;
    printf("%" PRIu16 " : %" PRIu16 "\n", shiftme, shiftcheck);
    _binary_left_shift(&shiftme, sizeof(uint16_t), 9);
    shiftcheck <<= 9;
    printf("%" PRIu16 " : %" PRIu16 "\n", shiftme, shiftcheck);
    

    printf("shift right\n");
    uint16_t shiftmer = 0xFFFF;
    shiftcheck = 0xFFFF;
    printf("%" PRIu16 " : %" PRIu16 "\n", shiftmer, shiftcheck);
    for (size_t i = 0; i < 15; i++)
    {
        _binary_right_shift(&shiftmer, sizeof(uint16_t), 1);
        shiftcheck >>= 1;
        printf("%" PRIu16 " : %" PRIu16 "\n", shiftmer, shiftcheck);
    }
    



    // bytes_example();
    // printf("\x64\n");

    // dict_example();

    // entropy_example();

    return 0;
}