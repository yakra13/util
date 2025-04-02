#pragma once

#include <stdint.h>
#include <stdio.h>
#include "uthash.h"
#include "esfheader.h"

typedef struct
{
    void* key;
    void* value;
    UT_hash_handle hh;
} hash_item;

static void _add_item(hash_item** dict, void* key, void* value, size_t keySize, size_t valueSize);
static void* _find_item(hash_item* dict, void* key, size_t keySize);
static void _remove_item(hash_item* dict, void* key, size_t keySize);
static void _free_dict(hash_item* dict);


#define DICT_GET_ITEM(dict, key) _find_item(dict, &(key), sizeof(key))
#ifdef _WIN32
    // Helper function to detect if it's a function pointer
    template<typename T>
    struct is_function_pointer
    {
        static constexpr bool value = false;
    };

    template<typename Ret, typename... Args>
    struct is_function_pointer<Ret (*)(Args...)>
    {
        static constexpr bool value = true;
    };

    // Get size of value based on whether it's a function pointer
    template<typename T>
    size_t get_size(T val) {
        if constexpr (is_function_pointer<T>::value)
        {
            return sizeof(void*); // Size of function pointer is typically sizeof(void*)
        }
        else 
        {
            return sizeof(val);
        }
    }
    #define DICT_ADD_ITEM(dict, key, value) _add_item(&(dict), &(key), &(value), sizeof(key), get_size(value))
#else
    #define DICT_ADD_ITEM(dict, key, value) _add_item(&(dict), &(key), &(value), sizeof(key), sizeof(value))
#endif
#define DICT_DEL_ITEM(dict, key) _remove_item(dict, &(key), sizeof(key))
#define DICT_FREE(dict) _free_dict(&(dict))


static void _add_item(hash_item** dict, void* key, void* value, size_t keySize, size_t valueSize)
{
    hash_item* new_item;

    new_item = (hash_item*)_find_item(*dict, key, keySize);
    
    if (new_item == NULL)
    {
        // TODO: malloc error check
        new_item = (hash_item*)malloc(sizeof(hash_item));
        
        new_item->key = malloc(keySize);
        new_item->value = malloc(valueSize);

        HASH_ADD(hh, *dict, key, keySize, new_item);
        memcpy(new_item->key, key, keySize);
    }

    memcpy(new_item->value, value, valueSize);
}

static void* _find_item(hash_item* dict, void* key, size_t keySize)
{
    hash_item* item = NULL;
    HASH_FIND(hh, dict, key, keySize, item);
    return item ? item->value : NULL;
}

static void _remove_item(hash_item* dict, void* key, size_t keySize)
{
    hash_item* item = (hash_item*)_find_item(dict, key, keySize);
    if (item == NULL)
        return;
    
    free(item->key);
    free(item->value);
    HASH_DEL(dict, item);
    free(item);
}

static void _free_dict(hash_item* dict)
{
    hash_item* cur;
    hash_item* tmp;

    HASH_ITER(hh, dict, cur, tmp)
    {
        free(cur->key);
        free(cur->value);
        HASH_DEL(dict, cur);
        free(cur);
    }
}