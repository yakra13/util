
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include "decodestring.h"

// UTF-8 and ascii?


int main(int argc, char** argv)
{
    const char* decode_string_pattern = "decode_2string(";
    const wchar_t* decode_wstring_pattern = L"decode_2wstring(";
    const size_t string_pattern_length = strlen(decode_string_pattern);
    const size_t wstring_pattern_length = wcslen(decode_wstring_pattern);

    setlocal(LC_ALL, "");

    FILE* f = fopen(argv[0], "r+");
    if(!f)
    {
        // TODO
        perror(decode_string("cant open file"));
        return 1;
    }
    decode_string("some string");

  
    // "state tracking"

    wint_t wc;
    while ((wc = fgetwc(f)) != WEOF)
    {
        if (wc == L'd')
        {
            if (wcsncmp(&wc, decode_wstring_pattern, wstring_pattern_length) == 0)
            {
                for (size_t i = 0; i < wstring_pattern_length; i++)
                {
                    wc = fgetwc(f);
                }
                
            }
            
        }
    }

    fclose(f);

    return 0;
    // find a "
    // ftell its position
    // extract to the next "
    // encrypt with argv[1]
    // replace in file
    // write the file
}