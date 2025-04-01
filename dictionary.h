#pragma once

#include <stdint.h>
#include <stdio.h>
#include "uthash.h"
#include "esfheader.h"

typedef struct
{
    void* key;
    // void (*value)(FILE* file, ObjectHeader* header);
    void* value;
    UT_hash_handle hh;
} hash_item;

typedef struct
{
    void* key;//uint16_t key;
    void* value;
    UT_hash_handle hh;
} uint16_void_dict;

// typedef struct _dictionary
// {
//     size_t key_size;
//     size_t value_size;
//     hash_item* dict;
//     void (*Add)(struct _dictionary* self, void* key, void* value);
// } Dictionary;


static void _add_item(hash_item** dict, void* key, void* value, size_t keySize, size_t valueSize);
static void* _find_item(hash_item* dict, void* key, size_t keySize);
static void _remove_item(hash_item* dict, void* key, size_t keySize);
static void _free_dict(hash_item* dict);

// void _add_dict_item(Dictionary* self, void* key, void* value);

#define DICT_GET_ITEM(dict, key) _find_item(dict, &(key), sizeof(key))
#define DICT_ADD_ITEM(dict, key, value) _add_item(&(dict), &(key), &(value), sizeof(key), sizeof(value))
#define DICT_DEL_ITEM(dict, key) _remove_item(&(dict), &(key), sizeof(key))
#define DICT_FREE(dict) _free_dict(&(dict))


// #define DICT_NEW(keyType, valueType) _new_dict(sizeof(keyType), sizeof(valueType))
// static Dictionary* _new_dict(size_t key_size, size_t value_size)
// {
//     Dictionary* d = (Dictionary*)malloc(sizeof(Dictionary));
//     d->key_size = key_size;
//     d->value_size = value_size;
//     d->dict = (hash_item*)malloc(sizeof(hash_item));
//     d->Add = _add_dict_item;

//     return d;
// }

// void _add_dict_item(Dictionary* self, void* key, void* value)
// {
//     _add_item(&(self->dict), key, value, self->key_size, self->value_size);
// }

static void _add_item(hash_item** dict, void* key, void* value, size_t keySize, size_t valueSize)
{
    hash_item* new_item;

    new_item = (hash_item*)_find_item(*dict, key, keySize);
    
    if (new_item == NULL)
    {
        // TODO: malloc error check
        new_item = (hash_item*)malloc(sizeof(hash_item));
        
        // new_item->key = malloc(keySize);
        // new_item->value = malloc(valueSize);
        new_item->key = key;

        // HASH_ADD(hh, *dict, key, keySize, new_item);
        HASH_ADD_KEYPTR(hh, *dict, key, keySize, new_item);
        // memcpy(new_item->key, key, keySize);
    }
    new_item->value = value;
    // memcpy(new_item->value, value, valueSize);
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

//void add_item(ObjTypeMap** dict, uint16_t key, void (*value)(FILE*, ObjectHeader*))
// #define DICT_ADD_ITEM(dictionary, key, add) DICT_ADD_KV(dictionary, &((add)->key), sizeof(&(add)->key), add)
// #define DICT_ADD_KV(dictionary, key, keySize, add) add_item(dictionary, key, keySize, add)
// void add_item(void* dictionary, void* key, size_t keySize, void* value) 
// {
//     void* s;

//     HASH_ADD(hh, dictionary, key, keySize, value);


//     HASH_FIND_INT(*dict, &key, s);
//     if (s == NULL)
//     {
//         s = (ObjTypeMap*)malloc(sizeof(ObjTypeMap));
//         s->key = key;
//         HASH_ADD_INT(*dict, key, s);
//     }
//     // Add the key if it doesnt exist, if it does just modify its value
//     s->value = value;

// }