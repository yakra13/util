
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SET_BIT_FLAG(flags, position) _set_bit_flag(&(flags), sizeof(flags) * 8, position);
#define TOGGLE_BIT_FLAG(flags, position) _toggle_bit_flag(&(flags), sizeof(flags) * 8, position);
#define ROTATE_BITS_LEFT(value, shift) _rotate_bits_left(&(value), sizeof(value), shift);
#define ROTATE_BITS_RIGHT(value, shift) _rotate_bits_right(&(value), sizeof(value), shift);
#define GET_BIT_STRING(value, buffer) _get_bit_string(&(value), sizeof(value), buffer);

#define _get_print_bit_macro(_1, _2, NAME, ...) NAME

#define _print_bit_string_1(value) _print_bits(&(value), sizeof(value), ' ');
#define _print_bit_string_2(value, end) _print_bits(&(value), sizeof(value), end);

#define PRINT_BIT_STRING(...) _get_print_bit_macro(__VA_ARGS__, _print_bit_string_2, _print_bit_string_1)(__VA_ARGS__)
// #define PRINT_BIT_STRING(value, ...) _print_bits(&(value), sizeof(value), ##__VA_ARGS__);

static void _set_bit_flag(void* flags, size_t width, size_t position);
static void _toggle_bit_flag(void* flags, size_t width, size_t position);
static void _rotate_bits_left(void* value, size_t size, size_t shift);
static void _rotate_bits_right(void* value, size_t size, size_t shift);
static void _get_bit_string(void* value, size_t value_size, char* buffer);
static void _print_bits(void* value, size_t size, const char end);

static void _set_bit_flag(void* flags, size_t width, size_t position)
{
    if (position >= width)
        return;
    
    uintmax_t temp = 0;
    
    memcpy(&temp, flags, width / 8);
    temp |= (1 << position);
    memcpy(flags, &temp, width / 8);
}

static void _toggle_bit_flag(void* flags, size_t width, size_t position)
{
    if (position >= width)
        return;
    
    uintmax_t temp = 0;

    memcpy(&temp, flags, width / 8);
    temp ^= (1 << position);
    memcpy(flags, &temp, width / 8);
}

static void _rotate_bits_left(void* value, size_t size, size_t shift)
{
    if (!value)
        return;

    shift %= size * 8;
    
    uintmax_t t = 0;
    memcpy(&t, value, size);
    t = (t << shift) | (t >> (size * 8 - shift));
    memcpy(value, &t, size); 
}

static void _rotate_bits_right(void* value, size_t size, size_t shift)
{
    if (!value)
        return;

    shift %= size * 8;
    
    uintmax_t t = 0;
    memcpy(&t, value, size);
    t = (t >> shift) | (t << (size * 8 - shift));
    memcpy(value, &t, size); 
}

static void _print_bits(void* value, size_t size, const char end)
{
    uintmax_t t = 0;
    
    int32_t bits = size * 8;

    char buffer[256] = { '\0' };

    memcpy(&t, value, size);

    for (int32_t i = 0; i < bits; i++)
    {
        buffer[i] = (t & (1U << bits - i - 1)) ? '1' : '0';
    }

    printf("%s", buffer);

    putchar(end);
}

static void _get_bit_string(void* value, size_t value_size, char* buffer)
{
    uintmax_t t = 0;
    int32_t bits = value_size * 8;
    
    memcpy(&t, value, value_size);

    for (int32_t i = 0; i < bits; i++)
    {
        buffer[i] = (t & (1U << bits - i - 1)) ? '1' : '0';
    }
}