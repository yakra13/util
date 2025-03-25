#pragma once

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void create_shellcode(unsigned char* data, size_t* length)
{
    memcpy(data, "something", 9);
    *length = 9;
}

void cs_return();
void cs_sys_exit();
void cs_call();

// build custom shell codes
void thing()
{
    unsigned char* b = (unsigned char*)malloc(100);

    unsigned char* p = b;

    unsigned char* data;
    size_t length = 0;

    //create small part of the shell code
    create_shellcode(data, &length);
    //copy it into the buffer at p
    memcpy(p, data, length);
    //move p by the amount copied
    p += length;

    //create more shell code and repeat this process

    //return the buffer shellcode to be injected etc
}