
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// NOTE: like none of this stuff takes into account endianness

#define SET_BIT_FLAG(flags, position) _set_bit_flag(&(flags), sizeof(flags) * 8, position);
#define TOGGLE_BIT_FLAG(flags, position) _toggle_bit_flag(&(flags), sizeof(flags) * 8, position);
#define ROTATE_BITS_LEFT(value, shift) _rotate_bits_left(&(value), sizeof(value), shift);
#define ROTATE_BITS_RIGHT(value, shift) _rotate_bits_right(&(value), sizeof(value), shift);
#define GET_BIT_STRING(value, buffer) _get_bit_string(&(value), sizeof(value), buffer);

#define GET_UINT_DIGIT_COUNT(value) _digit_count(&(value), sizeof(value))
#define GET_SINT_DIGIT_COUNT(value) _signed_digit_count(&(value), sizeof(value))
#define UINT_TO_ASCII(value, buffer) _uint_to_ascii(&(value), sizeof(value), buffer)
#define SINT_TO_ASCII(value, buffer) _sint_to_ascii(&(value), sizeof(value), buffer)
#define CONVERT_SINT_TO_UINT(value) _sint_to_uint(&(value), sizeof(value))


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

static int _digit_count(void* value, size_t size)
{
    uintmax_t v = 0;

    memcpy(&v, value, size);
    
    if (v == 0)
        return 1;
    
    return (int)floor(log10((double)v)) + 1;
}

static void* _sint_to_uint(void* value, size_t size)
{
    // Check for signed bit
    if((((unsigned char*)value)[size - 1] >> 7) & 1 != 1)
    {
        return value;
    }

    uint8_t carry = 0;
    // XOR each byte and add 1
    for (size_t i = 0; i < size; i++)
    {
        unsigned char* b = (unsigned char*)(value) + i;
        *b ^= 0xFF;
        
        if (i == 0 || carry == 1)
        {
            *b += 1;
            carry = *b == 0 ? 1 : 0;
        }
    }

    return value;
}

void _binary_left_shift(void* value, size_t size, size_t shift)
{
    size_t shift_bytes = shift / 8;
    size_t shift_bits  = shift % 8;
    
    unsigned char c_store = 0;

    for (size_t _ = 0; _ < shift_bytes; _++)
    {
        for (size_t offset = 0; offset < size; offset++)
        {
            unsigned char* b = ((unsigned char*)value) + offset;
            unsigned char c = *b;
            *b = c_store;
            c_store = c;
        }
    }

    c_store = 0;

    if (!shift_bits)
        return;

    for (size_t offset = 0; offset < size; offset++)
    {
        unsigned char* b = ((unsigned char*)value) + offset;
        unsigned char c = *b;
        c >>= 8 - shift_bits;
        *b <<= shift_bits;
        *b += c_store;
        c_store = c;
    }
}


void _binary_right_shift(void* value, size_t size, size_t shift)
{
    size_t shift_bytes = shift / 8;
    size_t shift_bits  = shift % 8;
    
    unsigned char c_store = 0;

    for (size_t _ = 0; _ < shift_bytes; _++)
    {
        for (int offset = size - 1; offset >= 0; offset--)
        {
            unsigned char* b = ((unsigned char*)value) + offset;
            unsigned char c = *b;
            *b = c_store;
            c_store = c;
        }
    }

    c_store = 0;

    if (!shift_bits)
        return;

    for (int offset = size - 1; offset >= 0; offset--)
    {
        unsigned char* b = ((unsigned char*)value) + offset;
        unsigned char c = *b;
        c <<= 8 - shift_bits;
        *b >>= shift_bits;
        *b += c_store;
        c_store = c;
    }
}

// static void* _unsigned_binary_division(void* dividend, size_t dividend_size, void* divisor, size_t divisor_size)
// {
//     void* quotient = calloc(dividend_size, sizeof(char));
//     void* remainder = calloc(dividend_size, sizeof(char));

//     // if divisor == 0:
//     // TODO

//     for (size_t i = dividend_size * 8 - 1; i >= 0; i--)
//     {
        

//     }
    
// }

static int _signed_digit_count(void* value, size_t size)
{
    uintmax_t v = 0;
    
    memcpy(&v, value, size);
    
    if (v == 0)
        return 1;

    // uintmax_t sign_bit = ((v >> (size * 8 - 1)) & 1);
    unsigned char sbit = ((unsigned char*)value)[size - 1] >> 7 == 1;

    if (sbit)
    {
        uintmax_t mask = 0;

        for (size_t i = 0; i < size * 8; i++)
        {
            mask += (v & (1 << i)) ^ (1 << i);
        }

        v = mask + 1;
    }

    int cnt = 1;
    uintmax_t temp = v;
    while (temp >= 10)
    {
        temp /= 10;
        cnt++;
    }

    return cnt;
    // return (int)floor(log10((double)v)) + 1;
}

static void _sint_to_ascii(void* value, size_t size, char* buffer)
{
    uintmax_t v = 0;
    memcpy(&v, value, size);

    uintmax_t sign_bit = ((v >> (size * 8 - 1)) & 1);

    int digit_count = _signed_digit_count(value, size);

    // Convert to unsigned
    if (sign_bit)
    {
        // XOR each byte and add 1
        for (size_t i = 0; i < size; i++)
        {
            *((unsigned char*)(&v) + i) ^= 0xFF;
        }
        
        v += 1;
        // Prepend the negative sign
        buffer[0] = '-';

        digit_count += 1;
    }

    // Add the null terminator
    buffer[digit_count] = '\0';

    while (digit_count > sign_bit)
    {
        buffer[--digit_count] = '0' + (v % 10);
        v /= 10;
    }
}

static void _uint_to_ascii(void* value, size_t size, char* buffer)
{
    uintmax_t v = 0;

    int digit_count = _digit_count(value, size);

    memcpy(&v, value, size);

    buffer[digit_count] = '\0';

    while (digit_count > -1)
    {
        buffer[--digit_count] = '0' + (v % 10);
        v /= 10;
    }
}