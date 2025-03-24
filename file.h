#ifndef ZFILE
#define ZFILE

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

char* normalize_path(const char* path);

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>

    #define MAX_PATH_LENGTH 260
    #define PATH_SEPARATOR '\\'

    // void hideFile(std::string filepath)
    // {
    //     int result = SetFileAttributesA(filepath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    //     // handle result...
    // }

    #define CREATE_DIR(path, ...) _mkdir(path)
    #define CREATE_DIR_R(path, ...) _mkdir_r(path, 0)
    // _chdir(path) change pwd
    int _mkdir_r(char* path)
    {
        // split the path
        // create each parent directory if not present
        return 0;        
    }

#elif __linux__
// A gather process info function? user pid command name...
    #include <unistd.h>

    #define MAX_PATH_LENGTH 4096
    #define PATH_SEPARATOR '/'

    #define CREATE_DIR(path, ...) mkdir(path, ##__VA_ARGS__ ? __VA_ARGS__ : S_IRWXU | S_IRWXG | S_IROTH)
    #define __CREATE_DIR_R_PATH(path, ...) _mkdir_r(path, S_IRWXU | S_IRWXG | S_IROTH)
    #define __CREATE_DIR_R_BOTH(path, mode) _mkdir_r(path, mode)

    #define __GET_MACRO(_1, _2, NAME, ...) NAME
    #define CREATE_DIR_R(...) __GET_MACRO(__VA_ARGS__, __CREATE_DIR_R_BOTH, __CREATE_DIR_R_PATH)(__VA_ARGS__)

#else

#endif

char* normalize_path(const char* path)
{
    static char resolved[MAX_PATH_LENGTH];

    #ifdef _WIN32
        if (_fullpath(resolved, path, MAX_PATH_LENGTH) == NULL)
        {
            return NULL;
        }
    #else
        if (realpath(path, resolved) == NULL)
        {
            return NULL;
        }
    #endif

    return resolved;
}

static int _mkdir_r(const char* path, mode_t mode = 0)
{
    char buffer[MAX_PATH_LENGTH] = { 0 };
    char* p = buffer;
    size_t pathLength = 0;
    // TODO: check if this removes trailing '/' '\'
    char* normalized = normalize_path(path);

    if (!normalized)
    {
        // TODO: couldnt normalize the path
        return -1;
    }

    strcpy(buffer, normalized);

    pathLength = strlen(buffer);

    // TODO possibly remove trailing '/' '\'

    #ifdef _WIN32
        if (pathLength > 2 && p[1] == ':' && p[2] == PATH_SEPARATOR)
            p += 3;
    #else
        if (p[0] == PATH_SEPARATOR)
            p++;
    #endif

    while ((p = strchr(p, PATH_SEPARATOR)) != NULL)
    {
        *p = '\0';

        if (CREATE_DIR(buffer, mode) != 0 && errno != EEXIST)
        {
            perror("TODO message");
            return -1;
        }

        *p = PATH_SEPARATOR;
        p++;
    }

    if (CREATE_DIR(buffer, mode) != 0 && errno != EEXIST)
    {
        perror("TODO message");
        return -1;
    }

    return 0;
}



static void _reversebytes(void*, size_t);
/// @brief Reverse the byte order of the passed value
/// @param value The variable to reverse
#define sreversebytes(value) _reversebytes(&(value), sizeof(value))

static void _reversebytes(void* value, size_t length)
{
	if (value == NULL || length == 0)
		return;

	uint8_t* p = (uint8_t*)value;

	for (size_t i = 0; i < length / 2; i++)
	{
		//store byte
		uint8_t t = p[i];
		//swap bytes
		p[i] = p[length - i - 1];
		p[length - i - 1] = t;
	}
}

static void _read_bytes(FILE*, void*, size_t);
#define sread_bytes(filePtr, dest) _read_bytes(filePtr, &(dest), sizeof(dest))
static void _read_bytes(FILE* file, void* dest, size_t size)
{
    if (!file || !dest || size == 0)
        return;
    
    size_t bytesRead = fread(dest, size, 1, file);
	// error check bytesRead: feof; ferror
}

static void _read_le(FILE*, void*, size_t);
/// @brief test
/// @param file 
/// @param dest 
#define sread_le(filePtr, out) _read_le(filePtr, &(out), sizeof(out))
static void _read_le(FILE* file, void* dest, size_t size)
{
    _read_bytes(file, dest, size);

	_reversebytes(dest, size);
}


void _print_as_byte_string(void*, size_t, unsigned char);
#define PRINT_VALUE_STRING(value, ...) _print_as_byte_string(&(value), sizeof(value), ##__VA_ARGS__)
void _print_as_byte_string(void* value, size_t size, unsigned char end = ' ')
{
    unsigned char* c = (unsigned char*)value;

    for (size_t i = 0; i < size; i++)
    {
        putchar(c[i]);
    }

    putchar(end);
} 

#endif