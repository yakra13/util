#pragma once

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifdef __linux__
    #include <locale.h>
#endif

#define NEW_TEXT(text) new_string(text, sizeof(text[0]) == sizeof(wchar_t));

typedef struct
{
    void* head;
    size_t length;
    size_t capacity;
    bool __is_wide;
} Text;

int to_narrow(Text* text);
void to_wide(Text* text);
void concat_text(Text* dest, Text* source);

static Text* new_string(const void* text, bool is_wide)
{
    Text* t = (Text*)malloc(sizeof(Text));

    if (t == NULL)
    {
        // TODO
        return NULL;
    }

    t->__is_wide = is_wide;

    t->length = t->__is_wide ? wcslen((const wchar_t*)text) : strlen((const char*)text);

    // TODO: better way to do capacity
    t->capacity = t->length * 2;

    t->head = calloc(t->capacity, t->__is_wide ? sizeof(wchar_t) : sizeof(char));

    if (t->head == NULL)
    {
        //TODO
        free(t);
        return NULL;
    }

    memcpy(t->head, text, t->__is_wide ? t->length * sizeof(wchar_t) : t->length * sizeof(char));

    return t;
}

void concat_text(Text* dest, Text* source)
{
    // Make a copy of the source to keep it intact
    Text tmp;
    tmp.__is_wide = source->__is_wide;
    tmp.length = source->length;
    tmp.capacity = source->capacity;
    tmp.head = calloc(tmp.capacity, tmp.__is_wide ? sizeof(wchar_t) : sizeof(char));

    memcpy(tmp.head, source->head, tmp.__is_wide ? tmp.length * sizeof(wchar_t) : tmp.length * sizeof(char));

    size_t total_length = dest->length + tmp.length;

    // Grow text if necessary
    if (total_length > dest->capacity)
    {
        void* t = realloc(dest->head, dest->__is_wide ? total_length * sizeof(wchar_t) : total_length * sizeof(char));
        if (t == NULL)
        {
            // TODO
        }
    }

    // Convert source copy string wideness to match dest 
    if (dest->__is_wide != tmp.__is_wide)
    {
        if (dest->__is_wide)
            to_wide(&tmp);
        else
            to_narrow(&tmp);
    }

    memcpy(dest->head + (dest->__is_wide ? dest->length * sizeof(wchar_t) : dest->length * sizeof(char)),
           tmp.head,
           tmp.__is_wide ? tmp.length * sizeof(wchar_t) : tmp.length * sizeof(char));

    free(tmp.head);
}

void to_wide(Text* text)
{
    if (text->__is_wide)
        return;

    wchar_t* tmp = (wchar_t*)calloc(text->capacity, sizeof(wchar_t));

    size_t r = mbstowcs(tmp, (char*)text->head, text->capacity);
    
    if (r == (size_t)-1)
    {
        // TODO
        // <limits.h> for SIZE_MAX
        // perror("Conversion failed");
        return;
    }

    free(text->head);
    text->head = tmp;
    text->__is_wide = true;
}

int to_narrow(Text* text)
{
    if (!text->__is_wide)
        return 1;

    char* tmp = (char*)calloc(text->capacity, sizeof(char));

    size_t r = wcstombs(tmp, (wchar_t*)text->head, text->capacity);

    if (r == (size_t)-1)
    {
        // TODO
        // <limits.h> for SIZE_MAX
        // perror("Conversion failed");
        return -1;
    }

    free(text->head);
    text->head = tmp;
    text->__is_wide = false;

    return 0;
}

void test()
{
    Text* t = NEW_TEXT(L"something");
}