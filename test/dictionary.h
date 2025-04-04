/****************************************************************************************
dictionary.h

compilation for static library - linux:
gcc -c dictionary.c -o dictionary.o
ar rcs libdict.a dictionary.o

compiling
gcc main.c -o main -L. -ldict
-L. look in local directory for library
-ldict -l replaces lib in libdict.a and .a is not required
****************************************************************************************/
#pragma once
#include <stddef.h>

// #include "uthash.h"

// extracted from uthash.h
typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;                       /* prev element in app order      */
   void *next;                       /* next element in app order      */
   struct UT_hash_handle *hh_prev;   /* previous hh in bucket order    */
   struct UT_hash_handle *hh_next;   /* next hh in bucket order        */
   const void *key;                  /* ptr to enclosing struct's key  */
   unsigned keylen;                  /* enclosing struct's key len     */
   unsigned hashv;                   /* result of hash-fcn(key)        */
} UT_hash_handle;

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

// Dictionary* _new_dictionary(size_t keySize, size_t valueSize);
// void _add_entry(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);
// void _set_value(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);
// dict_entry* _get_entry(Dictionary* dict, void* key, size_t keySize);
// void* _get_entry_value(Dictionary* dict, void* key, size_t keySize);
// void _remove_entry(Dictionary* dict, void* key, size_t keySize);
// void _free_dict(Dictionary* dict);

// Taken from uthash.h
/* These macros use decltype or the earlier __typeof GNU extension.
   As decltype is only available in newer compilers (VS2010 or gcc 4.3+
   when compiling c++ source) this code uses whatever method is needed
   or, for VS2008 where neither is available, uses casting workarounds. */
#if !defined(DECLTYPE) && !defined(NO_DECLTYPE)
#if defined(_MSC_VER)   /* MS compiler */
#if _MSC_VER >= 1600 && defined(__cplusplus)  /* VS2010 or newer in C++ mode */
#define DECLTYPE(x) (decltype(x))
#else                   /* VS2008 or older (or VS2010 in C mode) */
#define NO_DECLTYPE
#endif
#elif defined(__MCST__)  /* Elbrus C Compiler */
#define DECLTYPE(x) (__typeof(x))
#elif defined(__BORLANDC__) || defined(__ICCARM__) || defined(__LCC__) || defined(__WATCOMC__)
#define NO_DECLTYPE
#else                   /* GNU, Sun and other compilers */
#define DECLTYPE(x) (__typeof(x))
#endif
#endif

// #ifdef NO_DECLTYPE
// #define DICT_ITER(dict, entry, tmp) \
// for(((entry) = (dict->head)), ((*(char**)(&(tmp))) = (char*)((dict->head != NULL) ? (dict->head)->hh.next : NULL)); \
//     (entry) != NULL; \
//     ((entry) = (tmp)), ((*(char**)(&(tmp))) = (char*)((tmp != NULL) ? (tmp)->hh.next : NULL)))
// #else
// #define DICT_ITER(dict, entry, tmp) \
//     for(((entry) = (dict->head)), ((tmp) = DECLTYPE(entry)((dict->head != NULL) ? (dict->head)->hh.next : NULL)); \
//     (entry) != NULL; ((entry) = (tmp)), ((tmp) = DECLTYPE(entry)((tmp != NULL) ? (tmp)->hh.next : NULL)))
// #endif

#ifdef NO_DECLTYPE
#define BEGIN_DICT_ITER(dict, entry) { \
    dict_entry* entry = NULL; \
    dict_entry* tmp = NULL; \
for(((entry) = (dict->head)), ((*(char**)(&(tmp))) = (char*)((dict->head != NULL) ? (dict->head)->hh.next : NULL)); \
    (entry) != NULL; \
    ((entry) = (tmp)), ((*(char**)(&(tmp))) = (char*)((tmp != NULL) ? (tmp)->hh.next : NULL))) {
#define END_DICT_ITER() } }
#else
#define BEGIN_DICT_ITER(dict, entry) { \
    dict_entry* entry = NULL; \
    dict_entry* tmp = NULL; \
    for(((entry) = (dict->head)), ((tmp) = DECLTYPE(entry)((dict->head != NULL) ? (dict->head)->hh.next : NULL)); \
    (entry) != NULL; \
    ((entry) = (tmp)), ((tmp) = DECLTYPE(entry)((tmp != NULL) ? (tmp)->hh.next : NULL))) {
#define END_DICT_ITER() } }
#endif



/// @brief Create a new dictionary with the specified key and value types.
/// @param keyType Underlying type for keys
/// @param valueType Underlying type for values
/// @note Only the size of the underlying types is stored not the type itself.  
/// When accessing the key or value be sure to cast it to the appropriate type.
/// @return Dictionary*
#define DICT_NEW_DICT(keyType, valueType) new_dictionary(sizeof(keyType), sizeof(valueType));
#define DICT_ADD_ENTRY(dict, key, value) add_entry(dict, &(key), &(value), sizeof(key), sizeof(value));
#define DICT_GET_ENTRY(dict, key) get_entry(dict, &(key), sizeof(key));
#define DICT_SET_VALUE(dict, key, value) set_value(dict, &(key), &(value), sizeof(key), sizeof(value));
#define DICT_GET_VALUE(dict, key) get_entry_value(dict, &(key), sizeof(key));
#define DICT_DEL_ENTRY(dict, key) remove_entry(dict, &(key), sizeof(key));
#define DICT_FREE(dict) free_dict(dict);


Dictionary* new_dictionary(size_t keySize, size_t valueSize);

void add_entry(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);

void set_value(Dictionary* dict, void* key, void* value, size_t keySize, size_t valueSize);

dict_entry* get_entry(Dictionary* dict, void* key, size_t keySize);

void* get_entry_value(Dictionary* dict, void* key, size_t keySize);

void remove_entry(Dictionary* dict, void* key, size_t keySize);

void free_dict(Dictionary* dict);