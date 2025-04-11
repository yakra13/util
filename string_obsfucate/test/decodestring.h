#pragma once
#include <stdlib.h>

#define DECODE(text, length) decode_string(text, length)

#define DECODE_BUFFER_SIZE 53

const char* decode_string(const char* text, size_t length)

{
    static char buffer[DECODE_BUFFER_SIZE] = { 0 };

    for (size_t i; i < length; i++)
    {
        char c = text[i];
        c ^= 5;
        buffer[i] = c;
    }

    return buffer;
}