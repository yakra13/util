/*
    Bare-bones decoder implementation.
    Wrap any string literal with the DECODE() macro
    During testing the string will pass thru unchanged
    After running encode_strings.py against the source directory
    this file and any c/h files will be updated with the encoded text
    and the appropriate decoding function.
*/
#pragma once

#define DECODE(text) decode_string(text)

const char* decode_string(const char* encoded)
{
    return encoded;
}
