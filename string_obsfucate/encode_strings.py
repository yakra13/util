#!/usr/bin/env python3
import re
from typing import Literal, Callable
import argparse
from pathlib import Path
import glob
import inspect

EncodeFunc = Callable[[str], str]
_MAX_STRING_LENGTH = 0

def rotate_bits_left(value: int, n: int) -> int:
    value = value & 0xFF
    n = abs(n) % 8
    return ((value << n) & 0xFF) | (value >> (8 - n))

def rotate_bits_right(value: int, n: int) -> int:
    value = value & 0xFF
    n = abs(n) % 8
    return (value >> n) | ((value << (8 - n)) & 0xFF)

class Encoder():
    def encode(self, text: str) -> str:
        pass
    def get_decode_func(self) -> str:
        return ""

class BitRotLeft3(Encoder):
    def encode(self, text: str) -> str:
        out: str = ""
        for c in text:
            out += chr(rotate_bits_left(ord(c), 3))
        return out

    def get_decode_func(self) -> str:
        return '''
const char* decode_string(const char* encoded, size_t length)
{
    static char buffer[DECODE_BUFFER_SIZE] = { 0 };

    for (size_t i; i < length, i++)
    {
        char c = encodeed[i];
        c = (c >> 3) | (c << (8 - 3));
        buffer[i] = c;
    }

    return buffer;
}'''

def bit_rot_left3(text: str) -> str:
    out: str = ""
    for c in text:
        out += chr(rotate_bits_left(ord(c), 3))
    return out

def encode(text: str, func: EncodeFunc) -> str:
    # Encode the text and convert to hexidecimal notation
    o: str = ""
    for c in func(text):
        o += '\\x' + format(ord(c), 'x')
    return o

def insert_decoder(func: EncodeFunc):
    pass

def dothething(file_path: Path) -> bool:
    global _MAX_STRING_LENGTH
    # Get the contents of the file
    f = open(file_path.resolve(), 'r', encoding='utf-8')
    data: str = f.read()
    f.close()

    # Find all instances of 'decode_string' function
    matches = re.finditer(r'decode_string\("([^"]*)"\)', data)
    if len(matches) < 1:
        return False

    output: str = ""
    last = 0

    # Rebuild the file with encoded string literals
    for m in matches:
        s = m.start() + len('decode_string("')
        e = m.end() - len('")')
        l = e - s

        # Store the largest string length for use in the decoder header
        if l > _MAX_STRING_LENGTH:
            _MAX_STRING_LENGTH = l

        output += data[last:s]
        output += encode(data[s:e], bit_rot_left3)
        output += f'", {l}'
        last = e + 1

    output += data[last:]

    print(output)

    # backup the original

    # write the new file

    return True

class StringEncoder():
    _root_directory: Path   = None
    _max_string_length: int = 0
    _encoder: EncodeFunc    = None
    _files: list[Path]      = []
    _header: Path           = None

    def __init__(self, root_directory: Path, files: list[Path], encoder: EncodeFunc) -> None:
        '''Initialize StringEncoder'''
        for f in files:
            if f.name == "decodestring.h":
                if self._header is not None:
                    raise OSError("Multiple decodestring.h files found.")
                self._header = f
            else:
                self._files.append(f)

        self._root_directory = root_directory
        self._encoder = encoder

    def run() -> bool:


        return True



def get_functions():
    classes = [name for name, obj in globals().items() if isinstance(obj, type)]
    filtered_classes = [name for name in classes if issubclass(globals()[name], Encoder) and globals()[name] is not Encoder]
    # fn = [name for name, _ in cur_funcs]
    # print(fn)
    for n in filtered_classes:
        print(n)
        ## call function dynamically
        # globals()[n]()
        # instantiate
        cobj = globals()[n]
        instance:Encoder = cobj()
        # instance.encode("atat") # "safe call" for methods in the base class 'Encoder'
        if hasattr(instance, 'encode'): # check if method exists in derived class 
            instance.encode("atata")

    return filtered_classes


if __name__ == '__main__':

    arg_parser: argparse.ArgumentParser = argparse.ArgumentParser(description="TODO")
    
    arg_parser.add_argument("root", help="root dir help")
    arg_parser.add_argument("-e", "--encoding", choices=get_functions(), help="help for encoding")
    
    args = arg_parser.parse_args()

    dir = Path(args.root).resolve()
    files = list(dir.rglob('*.c')) + list(dir.rglob('*.h'))
    encoding = 'none'
    if args.encoding:
        encoding = args.encoding

    se: StringEncoder = StringEncoder(dir, files, encoder_map[encoding])

    print("print the names")
    get_functions()
    # get a list of the c and h files, seperate the decodestring.h file from the list
    # check each file for decode_string function and encode its string literal
    # backup all files that were encoded to _encode_backup or something, include a meta file for reverting changes
    # update decodestring.h file to include the c code for decoding

    # for f in filtered_files:
    #    print(f.name, "\t:\t", f.resolve())

    # for f in filtered_files:
    #     input("continue...")
    #     dothething(f)