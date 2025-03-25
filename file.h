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

    #define GET_CURRENT_DIR(buffer) getcwd(buffer, sizeof(buffer))
    #define GET_FILE_INFO(path) _get_file_info(path)

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

static int _mkdir_r(const char* path, unsigned int mode = 0)
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