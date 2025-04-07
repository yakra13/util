#include <stdio.h>
#include <stdint.h>
#include <wchar.h>
#include <locale.h>

#include "dictionary.h"
#include "file.h"

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

void text_example()
{
    // TODO: dont mix wide and narrow strings i guess...
    printf("set locale\n");
    fflush(stdout);
    setlocale(LC_ALL, "");

    Text* wide = NEW_TEXT(L"this is wide カタカナ")
    Text* narrow = NEW_TEXT("some more text");
    // concat_text(wide, narrow);

    wprintf(L"wide: %ls\n", (wchar_t*)wide->head);
    fflush(stdout);

    printf("narrow: %s\n", (char*)narrow->head);
    fflush(stdout);
    if (to_narrow(wide) != 0)
    {
        printf("it went wrong\n");
    }

    printf("narrow: %s\n", (char*)wide->head);
}

int main(int argc, char** argv)
{
    text_example();

    // dict_example();

    // entropy_example();

    return 0;
}