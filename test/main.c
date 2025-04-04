#include <stdio.h>
#include <stdint.h>
#include "dictionary.h"

// typedef int KEY_TYPE;
// typedef float VALUE_TYPE;


// #define CAST_AS_KEY(k) *(KEY_TYPE*)k
// #define CAST_AS_VALUE(v) *(VALUE_TYPE*)v

void dict_example()
{
    Dictionary* d = DICT_NEW_DICT(uint16_t, char*);

    uint16_t key = 1;
    char* value = "something here";
    char* value2 = "another thing";
    char* value3 = "i dunno";

    DICT_ADD_ENTRY(d, key, value)
    key = 2;
    DICT_ADD_ENTRY(d, key, value2)

    BEGIN_DICT_ITER(d, entry)
        printf("key: %hu, value: %s\n", *(uint16_t*)(entry->key), *(const char**)entry->value);
    END_DICT_ITER()

    key = 1;
    DICT_SET_VALUE(d, key, value3)

    BEGIN_DICT_ITER(d, entry)
        printf("key: %hu, value: %s\n", *(uint16_t*)(entry->key), *(const char**)entry->value);
    END_DICT_ITER()

    DICT_FREE(d)
}

void entropy_example()
{

}

int main(int argc, char** argv)
{
    dict_example();
    
    return 0;
}