#include "dictionary.h"

// Static function declarations
static Dictionary* _new_dictionary(size_t, size_t);

static void _add_entry(Dictionary*, void*, void*, size_t, size_t);

static dict_entry* _get_entry(Dictionary*, void*, size_t);

static void* _get_entry_value(Dictionary*, void*, size_t);

static void _remove_entry(Dictionary*, void*, size_t);

static void _free_dict(Dictionary*);

// Function wrappers for static functions
Dictionary* new_dictionary(size_t keySize, size_t valueSize) { return _new_dictionary(keySize, valueSize); }

void add_entry(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize) { return _add_entry(dict, key, value, keySize, valueSize); }

void set_value(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize) { return _set_value(dict, key, value, keySize, valueSize); }

dict_entry* get_entry(Dictionary* dict, void* key, size_t keySize) { return  _get_entry(dict, key, keySize); }

void* get_entry_value(Dictionary* dict, void* key, size_t keySize) { return  _get_entry_value(dict, key, keySize); }

void remove_entry(Dictionary* dict, void* key, size_t keySize) { return _remove_entry(dict, key, keySize); }

void free_dict(Dictionary* dict) { return _free_dict(dict); }

// Function definitions
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