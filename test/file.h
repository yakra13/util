/******************************************************************************
 Filename:    file.h
 Author:      John Doe
 Description: Header file defining buffer operations for the XYZ module.

Usage:
   - Include this header to perform various buffer operations.
   - You can adjust buffer sizes and other configuration values using preprocessor
     directives.


OS-Agnostic Macros
    CREATE_DIR(path, [mode])
        Windows - mode is ignored

    CREATE_DIR_R(path, [mode])
    GET_CURRENT_DIR(buffer)

Overridable Defines:
   - MAX_BUFFER_SIZE: The maximum buffer size in the module.
     Default value: 1024
     To override, define the constant before including this header:
       #define MAX_BUFFER_SIZE 2048

   - TIMEOUT: The timeout value for certain operations.
     Default value: 30
     To override, define the constant before including this header:
       #define TIMEOUT 60

Example Usage:
   #define MAX_BUFFER_SIZE 2048
   #include "buffer.h"

Function Declarations:
   - void initBuffer(void);        // Initializes the buffer with default size.
   - void resetBuffer(void);       // Resets the buffer contents to default.

 -----------------------------------------------------------------------------
 ******************************************************************************/

#ifndef ZFILE
#define ZFILE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <math.h>

typedef struct
{
    int32_t count;
    int32_t sizes[10];
} StructMap;

typedef struct _timespec
{
    long nsec;
    long sec;
} TimeSpec;

typedef struct _fileinfo
{
    TimeSpec accessTime;
    long blockSize;
    long blocks;
    TimeSpec createTime;
    unsigned long deviceID;
    unsigned int groupID;
    unsigned long inode;
    unsigned int permissions;
    TimeSpec modifyTime;
    unsigned long linkCount;
    unsigned long rDeviceID;
    long sizeInBytes;
    unsigned int userID;
    // fi.st_atim; //struct timespec
    //     fi.st_atim.tv_nsec; // long
    //     fi.st_atim.tv_sec; //time_t -> long
    // fi.st_blksize; //blksize_t long
    // fi.st_blocks; //blkcnt_t long  
    // fi.st_ctim; //struct timespec
    //     fi.st_ctim.tv_nsec; // long
    //     fi.st_ctim.tv_sec; //time_t long
    // fi.st_dev; //dev_t unsigned long
    // fi.st_gid; //gid_t unsigned int
    // fi.st_ino; //ino_t unsigned long
    // fi.st_mode; //mode_t unsigned int
    // fi.st_mtim; //struct timespec
    //     fi.st_mtim.tv_nsec; //long
    //     fi.st_mtim.tv_sec; //time_t long
    // fi.st_nlink; //nlink_t unsigned long
    // fi.st_rdev; //dev_t unsigned long
    // fi.st_size; //off_t long
    // fi.st_uid; //uid_t unsigned int
} FileInfo;

#ifdef _WIN32
    // #include <windows.h>
    #include <direct.h>

    #ifndef MAX_PATH_LENGTH
        #define MAX_PATH_LENGTH 260
    #endif

    #define PATH_SEPARATOR '\\'

    // void hideFile(std::string filepath)
    // {
    //     int result = SetFileAttributesA(filepath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    //     // handle result...
    // }


    #define CREATE_DIR(path, ...) _mkdir(path)
    #define CREATE_DIR_R(path, ...) _mkdir_r(path, 0)

    #define GET_CURRENT_DIR(buffer) _getcwd(buffer, sizeof(buffer))
    #define GET_FILE_INFO(path) _get_file_info(path)
    // _chdir(path) change pwd
    #define CHANGE_CWD(path) _change_cwd(path)

    typedef struct _stat _FileStat;
    /*
    struct _stat
    st_gid  	Numeric identifier of group that owns the file (UNIX-specific) This field will always be zero on Windows systems. A redirected file is classified as a Windows file.
    st_atime    Time of last access of file. Valid on NTFS but not on FAT formatted disk drives.
    st_ctime    Time of creation of file. Valid on NTFS but not on FAT formatted disk drives.
    st_dev  	Drive number of the disk containing the file (same as st_rdev).
    st_ino  	Number of the information node (the inode) for the file (UNIX-specific). On UNIX file systems, the inode describes the file date and time stamps, permissions, and content. When files are hard-linked to one another, they share the same inode. The inode, and therefore st_ino, has no meaning in the FAT, HPFS, or NTFS file systems.
    st_mode 	Bit mask for file-mode information. The _S_IFDIR bit is set if path specifies a directory; the _S_IFREG bit is set if path specifies an ordinary file or a device. User read/write bits are set according to the file's permission mode; user execute bits are set according to the filename extension.
    st_mtime    Time of last modification of file.
    st_nlink    Always 1 on non-NTFS file systems.
    st_rdev 	Drive number of the disk containing the file (same as st_dev).
    st_size 	Size of the file in bytes; a 64-bit integer for variations with the i64 suffix.
    st_uid  	Numeric identifier of user who owns file (UNIX-specific). This field will always be zero on Windows systems. A redirected file is classified as a Windows file.
    */


#else
// A gather process info function? user pid command name...
    #include <unistd.h>

    #ifndef MAX_PATH_LENGTH
        #define MAX_PATH_LENGTH 4096
    #endif

    #define PATH_SEPARATOR '/'

    #define __CREATE_DIR_PATH(path, ...) mkdir(path, S_IRWXU | S_IRWXG | S_IROTH)
    #define __CREATE_DIR_BOTH(path, mode) mkdir(path, mode)
    #define __CREATE_DIR_R_PATH(path, ...) _mkdir_r(path, S_IRWXU | S_IRWXG | S_IROTH)
    #define __CREATE_DIR_R_BOTH(path, mode) _mkdir_r(path, mode)

    #define __GET_MACRO(_1, _2, NAME, ...) NAME
    #define CREATE_DIR(...) __GET_MACRO(__VA_ARGS__, __CREATE_DIR_BOTH, __CREATE_DIR_PATH)(__VA_ARGS__)
    #define CREATE_DIR_R(...) __GET_MACRO(__VA_ARGS__, __CREATE_DIR_R_BOTH, __CREATE_DIR_R_PATH)(__VA_ARGS__)


    #define CHANGE_CWD(path) _change_cwd(path);
    #define GET_CURRENT_DIR(buffer) getcwd(buffer, sizeof(buffer));
    #define GET_FILE_INFO(path) _get_file_info(path);

    typedef struct stat _FileStat;
#endif


// char* normalize_path(const char* path)
// {
//     static char resolved[MAX_PATH_LENGTH];

//     #ifdef _WIN32
//         if (_fullpath(resolved, path, MAX_PATH_LENGTH) == NULL)
//         {
//             return NULL;
//         }
//     #else
//         if (realpath(path, resolved) == NULL)
//         {
//             return NULL;
//         }
//     #endif

//     return resolved;
// }



static FileInfo _get_file_info(const char* path)
{
    FileInfo out;
    //TODO: compare to windows types for conversion
    #ifdef _WIN32
        struct _stat fi;
        _stat(path, &fi);
    #else
        struct stat fi;
        stat("test.txt", &fi);
        out.accessTime.nsec = fi.st_atim.tv_nsec;
        out.accessTime.sec = fi.st_atim.tv_sec;
    #endif

    fi.st_atim; //struct timespec
        fi.st_atim.tv_nsec; // long
        fi.st_atim.tv_sec; //time_t -> long
    fi.st_blksize; //blksize_t long
    fi.st_blocks; //blkcnt_t long  
    fi.st_ctim; //struct timespec
        fi.st_ctim.tv_nsec; // long
        fi.st_ctim.tv_sec; //time_t long
    fi.st_dev; //dev_t unsigned long
    fi.st_gid; //gid_t unsigned int
    fi.st_ino; //ino_t unsigned long
    fi.st_mode; //mode_t unsigned int
    fi.st_mtim; //struct timespec
        fi.st_mtim.tv_nsec; //long
        fi.st_mtim.tv_sec; //time_t long
    fi.st_nlink; //nlink_t unsigned long
    fi.st_rdev; //dev_t unsigned long
    fi.st_size; //off_t long
    fi.st_uid; //uid_t unsigned int

    // st_atime
    //!st_blksize
    //!st_blocks
    // st_ctime
    // st_dev
    // st_gid
    // st_ino
    // st_mode
    // st_mtime
    // st_nlink
    // st_rdev
    // st_size
    // st_uid

    return out;
}

static bool _change_cwd(const char* path)
{
    int result = 0;
    #ifdef _WIN32
        result = _chdir(path);
    #endif
        result = chdir(path);
    
    if (result != 0)
    {
        // TODO
        //perror()
        return false;
    }

    return true;
}

static int _mkdir_r(const char* path, unsigned int mode)
{
    char buffer[MAX_PATH_LENGTH] = { 0 };
    char* p = buffer;
    size_t pathLength = 0;

    int result = 0;

    while (
    #ifdef _WIN32
        _fullpath(buffer, path, MAX_PATH_LENGTH) == NULL
    #else
        realpath(path, buffer) == NULL
    #endif
    )
    {
        if (result = (CREATE_DIR(buffer, mode)) != 0)
        {
            //TODO
            return result;
        }
    }

    // // TODO: check if this removes trailing '/' '\'
    // char* normalized = normalize_path(path);

    // if (!normalized)
    // {
    //     // TODO: couldnt normalize the path
    //     return -1;
    // }

    // strcpy(buffer, normalized);

    // pathLength = strlen(buffer);

    // // TODO possibly remove trailing '/' '\'

    // #ifdef _WIN32
    //     if (pathLength > 2 && p[1] == ':' && p[2] == PATH_SEPARATOR)
    //         p += 3;
    // #else
    //     if (p[0] == PATH_SEPARATOR)
    //         p++;
    // #endif

    // while ((p = strchr(p, PATH_SEPARATOR)) != NULL)
    // {
    //     *p = '\0';

    //     if (CREATE_DIR(buffer, mode) != 0 && errno != EEXIST)
    //     {
    //         perror("TODO message");
    //         return -1;
    //     }

    //     *p = PATH_SEPARATOR;
    //     p++;
    // }

    // if (CREATE_DIR(buffer, mode) != 0 && errno != EEXIST)
    // {
    //     perror("TODO message");
    //     return -1;
    // }

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

static void _reverse_struct_bytes(void*, size_t, const uint8_t*, size_t);
#define REVERSE_STRUCT_BYTES(structData, structMap) _reverse_struct_bytes(&(structData), sizeof(structData), structMap, sizeof(structMap))
static void _reverse_struct_bytes(void* structData, size_t dataSize, const uint8_t* map, size_t mapSize)
{
    size_t offset = 0;

    int32_t sum = 0;
    // Sum the map values
    for (int32_t i = 0; i < mapSize; i++)
    {
        sum += map[i];
    }
    
    // Check the values in map add up to the size of the struct
    if (dataSize != sum)
    {
        // TODO: error return vals?

        return;
    }

    for (int32_t i = 0; i < mapSize; i++)
    {
        _reversebytes((uint8_t*)structData + offset, map[i]);
        offset += map[i];
    }
    
}

static void _read_bytes(FILE*, void*, size_t);
#define sread_bytes(filePtr, dest) _read_bytes(filePtr, &(dest), sizeof(dest))
static void _read_bytes(FILE* file, void* dest, size_t size)
{
    if (!file || !dest || size == 0)
        return;

    //TODO: use fseek(file, offset, SEEK_SET) to read at an offset
    // ftell(file) to get current offset
    
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
void _print_as_byte_string(void* value, size_t size, unsigned char end)
{
    unsigned char* c = (unsigned char*)value;

    for (size_t i = 0; i < size; i++)
    {
        putchar(c[i]);
    }

    putchar(end);
} 

#endif

/*
FILE HEADER:
            |    FJBO   | obj count | file type |  unknown  |         offset        |        unknown        |
            |46 4a 42 4f|01 00 00 00|4f ab 00 00|00 00 00 00|20 00 00 00 00 00 00 00|ff ff ff ff ff ff ff ff|
OBJECT HEADER:
            |type | ver |   length  |  children |      data
    root    |00 80|00 00|d7 aa 70 3b|02 00 00 00|
   world    |00 81|00 00|17 62 70 3b|af 00 00 00|
    zone    |00 30|01 00|4c 7d 22 00|04 00 00 00|
 zone resc  |00 31|00 00|0c bc 21 00|04 01 00 00|
mat palette |10 11|01 00|f7 0d 03 00|03 00 00 00|
  mat p hdr |11 11|01 00|04 00 00 00|00 00 00 00|a3 5b b2 0a < id?
surface arr |01 10|00 00|d3 0c 03 00|03 00 00 00|
                                                |                            surface header                             |   palette color cnt in length   |
                                                |     id    |   width   |   height  |   depth   |    mip    | color cnt |   RGBA    |
  surface   |00 10|00 00|8d 59 00 00|00 00 00 00|4d 67 c9 43|80 00 00 00|80 00 00 00|01 00 00 00|08 00 00 00|00 01 00 00|07 34 38 ff|02363aff023a3dff063a3eff083a3fff183c3fff153a3dff063b40ff063c41ff0c3e43ff093e42ff0b3f44ff143e42ff1b3e41ff0b4044ff0e4146ff0c4045ff0f4246ff134346ff124246ff104347ff154246ff1b4245ff174348ff114348ff124549ff134449ff154549ff15464aff16474bff15474cff16474cff1a474cff19464aff1d4649ff17484bff16494dff17484cff1d494cff19494eff18494dff1a4a4fff1e4a4eff1c4b4fff0a4e53ff164e52ff1d4f54ff1b4b50ff1e4b50ff
            
     **0083 - 5A10 -> should be texture data? (surface)       
           049B end palette colors 
            049C number of mip maps, start with width x height pixels, then divide by 2 for each mip map
            128x128  4000
            64x64    1000
            32x32     400
            16x16     100
            8x8        40
            4x4        10
            2x2         4
            1x1         1
                     5555 ->  59F0 (end of surface data?)
            

            049C - 80 00 00 00
            44A0 - 40 00 00 00
            54A4 - 20 00 00 00
            58A8 - 10 00 00 00
            59AD - 08 00 00 00
            59F0 - 04 00 00 00 (mip size) 16 bytes
            5A04 - 02 00 00 00             4 bytes
            5A0C - 01 00 00 00             1 byte
            5A11 - 00 10 00 00 - next surface header
            
            5E35 end surface 2 palette



*/

#include <math.h>

double calculate_entropy(const char* filepath)
{
    FILE* f = fopen(filepath, "rb");
    if (!f)
    {
        //TODO
        return -1.0;
    }

    unsigned long long b_counts[256] = { 0 };
    unsigned char b;
    unsigned long long total_bytes = 0;

    while (fread(&b, 1, 1, f) == 1)
    {
        b_counts[b]++;
        total_bytes++;
    }

    fclose(f);

    double e = 0.0;

    for (int i = 0; i < 256; i++)
    {
        if (b_counts[i] > 0)
        {
            double p = (double)b_counts[i] / total_bytes;
            e -= p * log2(p);
        }
    }

    return e;    
}