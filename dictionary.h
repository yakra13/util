/****************************************************************************************
dictionary.h

Wrapper functions and macros for uthash.h *see uthash.h for copyright*

hash_item -> struct to handle generically typed key value pairs

Typical Usage:
    hash_item* dictionary = NULL;

    // Keys and values can not be literals due to the underlying void* types
    _anyType key = <key>;
    _anyType value = <value>;

    // Add a key and its value to the dictionary
    DICT_ADD_ENTRY(dictionary, key, value);

    // Get the stored hash_item* based on the key
    hash_item* item = DICT_GET_ITEM(dictionary, key);

    // Use the value be sure to cast it and dereference as necessary
    printf("Print the value %s\n", *(_anyType*)(item->value);

    // Remove entries as needed
    DICT_DEL_ITEM(dictionary, key);

    // Iterate over dictionary entries
    DICT_FOR_EACH(dictionary, entry) // 'entry' is whatever label you want to use, ie item, keyvalue, e, etc.
    {
        entry->key;
        entry->value;
    }

    // Clean up
    DICT_FREE(dict);


****************************************************************************************/
#pragma once

#include "uthash.h"

typedef struct
{
    void* key;
    void* value;
    UT_hash_handle hh;
} dict_entry;

typedef struct
{
    dict_entry* head;
    size_t key_size;
    size_t value_size;
    size_t count;
} Dictionary;

static Dictionary* _new_dictionary(size_t keySize, size_t valueSize);
static void _add_entry(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);
static void _set_value(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);
static dict_entry* _get_entry(Dictionary* dict, void* key, size_t keySize);
static void* _get_entry_value(Dictionary* dict, void* key, size_t keySize);
static void _remove_entry(Dictionary* dict, void* key, size_t keySize);
static void _free_dict(Dictionary* dict);

/// @brief Create a new dictionary with the specified key and value types.
/// @param keyType Underlying type for keys
/// @param valueType Underlying type for values
/// @note Only the size of the underlying types is stored not the type itself.  
/// When accessing the key or value be sure to cast it to the appropriate type.
/// @return Dictionary*
#define DICT_NEW_DICT(keyType, valueType) _new_dictionary(sizeof(keyType), sizeof(valueType))
#define DICT_ADD_ENTRY(dict, key, value) _add_entry(dict, &(key), &(value), sizeof(key), sizeof(value))
#define DICT_GET_ENTRY(dict, key) _get_entry(dict, &(key), sizeof(key))
#define DICT_SET_VALUE(dict, key, value) _set_value(dict, &(key), &(value), sizeof(key), sizeof(value))
#define DICT_GET_VALUE(dict, key) _get_entry_value(dict, &(key), sizeof(key))
#define DICT_DEL_ENTRY(dict, key) _remove_entry(dict, &(key), sizeof(key))
#define DICT_FREE(dict) _free_dict(dict)

/// @brief Iterate each entry in the dictionary.
/// @param dict something
/// @param entryLabel something
/// @warning You must wrap this macro in its own scope.
/// 
/// Example usage:
///
/// @code
/// { DICT_FOR_EACH(dict) { printf("%d\\n", *(int*)__entry->value); } }
/// @endcode
#define DICT_FOR_EACH(dict, entryLabel) \
    dict_entry* entryLabel = NULL; \
    dict_entry* __entry_tmp = NULL; \
    HASH_ITER(hh, dict->head, entryLabel, __entry_tmp)


static Dictionary* _new_dictionary(size_t keySize, size_t valueSize)
{
    Dictionary* d = (Dictionary*)malloc(sizeof(Dictionary));
    d->count = 0;
    d->head = NULL;
    d->key_size = keySize;
    d->value_size = valueSize;

    return d;
}

static void _add_entry(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize)
{
    dict_entry* e = NULL;

    if (dict->key_size != keySize || dict->value_size != valueSize)
    {
        //TODO: provided key or value size mismatch
        return;
    }

    e = _get_entry(dict, key, keySize);
    
    if (e == NULL)
    {
        e = (dict_entry*)malloc(sizeof(dict_entry));
        if (!e)
        {
            //perror("Memory allocation failed in dictionary _add_item");
            return;
        }
        
        e->key = malloc(keySize);
        e->value = malloc(valueSize);

        memcpy(e->key, key, keySize);
        HASH_ADD_KEYPTR(hh, dict->head, e->key, keySize, e);

        dict->count++;
    }

    memcpy(e->value, value, valueSize);
}

static void _set_value(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize)
{
    dict_entry* e = NULL;

    e = _get_entry(dict, key, keySize);
    
    if (!e || dict->value_size != valueSize)
    {
        //TODO: key not in dictionary or size mismatch
        return;
    }
    // Copy the value
    memcpy(e->value, value, valueSize);
}

static dict_entry* _get_entry(Dictionary* dict, void* key, size_t keySize)
{
    dict_entry* e = NULL;
    HASH_FIND(hh, dict->head, key, keySize, e);
    return e;
}

static void* _get_entry_value(Dictionary* dict, void* key, size_t keySize)
{
    dict_entry* e = NULL;
    HASH_FIND(hh, dict->head, key, keySize, e);
    return e ? e->value : NULL;
}

static void _remove_entry(Dictionary* dict, void* key, size_t keySize)
{
    dict_entry* e = _get_entry(dict, key, keySize);

    if (e == NULL)
        return;
    
    free(e->key);
    free(e->value);
    HASH_DEL(dict->head, e);
    free(e);

    dict->count--;
}

static void _free_dict(Dictionary* dict)
{
    dict_entry* cur = NULL;
    dict_entry* tmp = NULL;

    HASH_ITER(hh, dict->head, cur, tmp)
    {
        if (cur)
        {
            free(cur->key);
            free(cur->value);
            HASH_DEL(dict->head, cur);
            free(cur);
        }
    }

    free(dict);
}