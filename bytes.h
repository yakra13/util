#ifndef ZBYTES
#define ZBYTES

// #include <cstdint>
// #include <type_traits>
// #include <cstring>
// #include <algorithm>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// template <typename T>
// inline T reverse_endian(T value)
// {
//     char bytes[sizeof(T)];

//     memcpy(bytes, &value, sizeof(T));

//     std::reverse(std::begin(bytes), std::end(bytes));

//     T reversed;
//     memcpy(&reversed, bytes, sizeof(T));

//     return reversed;
// }

// inline bool is_little_endian()
// {
//     uint16_t x = 1;
//     return *reinterpret_cast<uint8_t*>(&x) == 1;
// }


inline bool is_le_c()
{
    uint16_t x = 1;
    uint8_t* r = (uint8_t*)&x;

    return (*r == 1);
}

// template <typename T>
// static bool is_valid_flag_position(uint16_t position)
// {
//     std::size_t s = sizeof(T) * 8;

//     // Validate position is between 0 and bit count of T
//     if (s <= position || position < 0)
//         return false;
    
//     return true;
// }

// /// @brief Set a bit flag on
// /// @tparam T Integer types only
// /// @param flags The bit flags to operate on
// /// @param position 0-indexed bit position to turn on
// template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// inline void set_flag(T& flags, uint16_t position)
// {
//     if (!is_valid_flag_position<T>(position))
//         return;

//     flags |= (1 << position);
// }

static void _set_flag(void* flags, size_t width, size_t position);
#define sset_flag(flags, position) _set_flag(&(flags), sizeof(flags) * 8, position)
static void _set_flag(void* flags, size_t width, size_t position)
{
    if (position >= width)
        return;
    
    uintmax_t temp = 0;
    
    memcpy(&temp, flags, width / 8);
    temp |= (1 << position);
    memcpy(flags, &temp, width / 8);
}

static void _toggle_flag(void* flags, size_t width, size_t position);
#define stoggle_flag(flags, position) _toggle_flag(&(flags), sizeof(flags) * 8, position)
static void _toggle_flag(void* flags, size_t width, size_t position)
{
    if (position >= width)
        return;
    
    uintmax_t temp = 0;

    memcpy(&temp, flags, width / 8);
    temp ^= (1 << position);
    memcpy(flags, &temp, width / 8);
}

uint16_t rot_left16(uint16_t value, uint8_t shift)
{
    shift %= 16;
    return (value << shift) | (value << (16 - shift));
}

static void _rot_left(void* value, size_t size, size_t shift);
#define srot_left(value, shift) _rot_left(&(value), sizeof(value), shift)
static void _rot_left(void* value, size_t size, size_t shift)
{
    if (!value)
        return;

    shift %= size * 8;
    
    uintmax_t t = 0;
    memcpy(&t, value, size);
    t = (t << shift) | (t >> (size * 8 - shift));
    memcpy(value, &t, size); 
}

static void _rot_right(void* value, size_t size, size_t shift);
#define srot_right(value, shift) _rot_right(&(value), sizeof(value), shift)
static void _rot_right(void* value, size_t size, size_t shift)
{
    if (!value)
        return;

    shift %= size * 8;
    
    uintmax_t t = 0;
    memcpy(&t, value, size);
    t = (t >> shift) | (t << (size * 8 - shift));
    memcpy(value, &t, size); 
}

static void _print_binary(void* value, size_t size, unsigned char);
#define PRINT_BINARY_STRING(value, ...) _print_binary(&(value), sizeof(value), ##__VA_ARGS__)
static void _print_binary(void* value, size_t size, unsigned char end = ' ')
{
    uintmax_t t = 0;
    size_t bits = size * 8;
    // char* buffer = (char*)calloc(bits + 1, sizeof(char));

    // if (!buffer)
    // {
    //     // TODO:
    //     return;
    // }

    memcpy(&t, value, size);

    for (size_t i = 0; i < bits; i++)
    {
        //buffer[i] =
        //
        putchar((t & (1ULL << (bits - i - 1))) ? '1' : '0');
    }

    // buffer[bits] = '\0';

    // printf("%s", buffer);
    putchar(end);

    // free(buffer);
}

// void set_flag16(uint16_t flags, uint8_t flag)
// {
//     flag %= 16;
//     flags |= (1 << flag);

//     //rotate bits this rotates the bits left and takes the front bits that fall off onto the back of the value
//     // (x << pos) | (x >> (8 - pos))
// }

// /// @brief Set a bit flag off
// /// @tparam T Integer types only
// /// @param flags The bit flags to operate on
// /// @param position 0-indexed bit position to turn off
// template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// inline void clear_flag(T& flags, uint16_t position)
// {
//     if (!is_valid_flag_position<T>(position))
//         return;

//     flags &= ~(1 << position);
// }

// /// @brief Toggle a bit flag
// /// @tparam T Integer types only
// /// @param flags The bit flags to operate on
// /// @param position 0-indexed bit position to toggle
// template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// inline void toggle_flag(T& flags, uint16_t position)
// {
//     if (!is_valid_flag_position<T>(position))
//         return;

//     flags &= ~(1 << position);
// }

// /// @brief Check if flag is set
// /// @tparam T Integer types only
// /// @param flags The bit flags to operate on
// /// @param position 0-indexed bit position to check
// template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// inline bool is_flag_set(T& flags, uint16_t position)
// {
//     if (!is_valid_flag_position<T>(position))
//         return false;

//     return flags & (1 << position) ? 1 : 0;
// }

// /// @brief Check if flags match a mask
// /// @tparam T Integer types only
// /// @param flags The bit flags to operate on
// /// @param mask Mask to compare
// template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
// inline bool are_flags_set(T& flags, T mask)
// {
//     return (flags & mask) == mask;
// }

#endif