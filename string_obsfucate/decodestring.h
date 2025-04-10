#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#ifndef DECODE_BUFFER_SIZE
    #define DECODE_BUFFER_SIZE 1024
#endif

const char* decode_string(const char* encoded)
{
    // static char buffer[DECODE_BUFFER_SIZE] = { 0 };
    // size_t length = strlen(encoded);

    // // do decode here
    // // buffer[i] = encoded[i] ^ XOR_KEY;

    // buffer[length] = '\0'; // null terminate

    // return buffer;

    return encoded;
}

const wchar_t* decode_wstring(const wchar_t* encoded)
{
    // static wchar_t buffer[DECODE_BUFFER_SIZE] = { 0 };
    // size_t length = wcslen(encoded);

    // // do decode here
    // // buffer[i] = encoded[i] ^ XOR_KEY;

    // buffer[length] = L'\0'; // null terminate

    // return buffer;

    return encoded;
}
