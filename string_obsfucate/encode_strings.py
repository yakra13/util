#!/usr/bin/env python3
import re
from typing import Type, Final
import argparse
from pathlib import Path
import inspect
import shutil

# Utility functions

def rotate_bits_left(value: int, n: int) -> int:
    value = value & 0xFF
    n = abs(n) % 8
    return ((value << n) & 0xFF) | (value >> (8 - n))

def rotate_bits_right(value: int, n: int) -> int:
    value = value & 0xFF
    n = abs(n) % 8
    return (value >> n) | ((value << (8 - n)) & 0xFF)

# Encoders

class Encoder():
    _TEXT_PARAM: Final[str]   = 'text'
    _LENGTH_PARAM: Final[str] = 'length'
    _BUFFER_SIZE_DEFINE: Final[str] = 'DECODE_BUFFER_SIZE'
    _MACRO_NAME: Final[str]         = 'DECODE'
    _FUNC_NAME: Final[str]          = 'decode_string'
    _FUNC_SIGNATURE: Final[str]     = f'const char* {_FUNC_NAME}(const char* {_TEXT_PARAM}, size_t {_LENGTH_PARAM})'

    _func_def: str = ""

    def encode(self, text: str) -> str:
        '''Encodes the supplied text with whatver algorithm to be implemented in a derived class.'''
        pass

    def get_includes(self) -> list[str]:
        '''Return a list of includes for the c file. Call super to get <stdlib.h> for size_t type.
        Append specific includes needed for the particular encoder.
        '''
        return ['<stdlib.h>']

    def get_func_def(self) -> str:
        return self._func_def

class BitRotLeft3(Encoder):
    __ROT_STEPS: Final[int] = 3
    
    def __init__(self):
        self._func_def = \
f'''{self._FUNC_SIGNATURE}
{{
    static char buffer[{self._BUFFER_SIZE_DEFINE}] = {{ 0 }};

    for (size_t i; i < {self._LENGTH_PARAM}; i++)
    {{
        char c = {self._TEXT_PARAM}[i];
        c = (c >> {self.__ROT_STEPS}) | (c << (8 - {self.__ROT_STEPS}));
        buffer[i] = c;
    }}

    return buffer;
}}'''

    def encode(self, text: str) -> str:
        out: str = ""

        for c in text:
            out += chr(rotate_bits_left(ord(c), self.__ROT_STEPS))

        return out

    def get_includes(self) -> list[str]:
        return super().get_includes()

    def get_func_def(self) -> str:
        return super().get_func_def()


class XOR(Encoder):
    __XOR_VALUE: Final[int] = 0x5

    def __init__(self):
        self._func_def = \
f'''{self._FUNC_SIGNATURE}
{{
    static char buffer[{self._BUFFER_SIZE_DEFINE}] = {{ 0 }};

    for (size_t i; i < {self._LENGTH_PARAM}; i++)
    {{
        char c = {self._TEXT_PARAM}[i];
        c ^= {self.__XOR_VALUE};
        buffer[i] = c;
    }}

    return buffer;
}}'''

    def encode(self, text: str) -> str:
        out: str = ''

        for c in text:
            out += chr(ord(c) ^ self.__XOR_VALUE)

        return out
    
    def get_includes(self) -> list[str]:
        return super().get_includes()
    
    def get_func_def(self) -> str:
        return super().get_func_def()


class StringEncoder():
    ''' StringEncoder
    '''
    _BUFFER_SIZE_DEFINE: Final[str]    = 'DECODE_BUFFER_SIZE'
    _DECODE_STRING_PATTERN: Final[str] = 'DECODE'
    _DECODE_SIGNATURE: Final[str]      = 'const char* decode_string(const char* text, size_t length)\n{\n'
    _DECODE_HEADER_NAME: Final[str]    = 'decodestring.h'
    _BACKUP_EXTENSION: Final[str]      = '.bak'

    _root_directory: Path   = None
    _max_string_length: int = 0
    _encoder: Encoder       = None
    _files: list[Path]      = []
    _header: Path           = None

    def __init__(self, root_directory: Path, files: list[Path], encoder: str) -> None:
        '''Initialize StringEncoder'''
        for f in files:
            if f.name == self._DECODE_HEADER_NAME:
                if self._header is not None:
                    raise OSError(f"Multiple {self._DECODE_HEADER_NAME} files found.")
                self._header = f
            else:
                self._files.append(f)

        self._root_directory = root_directory
        class_obj = globals()[encoder]
        self._encoder = class_obj()

    def run(self) -> None:
        for file in self._files:
            # Get the contents of the file
            f = open(file.resolve(), 'r', encoding='utf-8')
            data: str = f.read()
            f.close()

            # Find all instances of 'decode_string' function
            matches = list(re.finditer(fr'{self._DECODE_STRING_PATTERN}\("([^"]*)"\)', data))

            if len(matches) < 1:
                continue
            
            # Backup the current file in place with .bak extension
            shutil.copy(file.resolve(), str(file.resolve()) + self._BACKUP_EXTENSION)

            output: str = ""
            last: int = 0

            # Rebuild the file with encoded string literals
            for m in matches:
                # NOTE: Currently assumed there are no spaces ie DECODE("... and not DECODE ( "...
                s = m.start() + len(f'{self._DECODE_STRING_PATTERN}("')
                e = m.end() - len('")')
                l = e - s

                # Store the largest string length for use in the decoder header
                if l > self._max_string_length:
                    self._max_string_length = l

                # Fill in with the characters before the string literal
                output += data[last:s]

                # Encode the string literal and convert to hexidecimal notation
                for c in self._encoder.encode(data[s:e]):
                    output += '\\x' + format(ord(c), 'x')

                # Append the length parameter to the decode_string function
                output += f'", {l}'
                last = e + 1
            
            output += data[last:]
            
            # Overwrite the original file
            with open(file.resolve(), "w", encoding='utf-8') as f:
                f.write(output)

        decoder_function_text: str = "#pragma once\n"
        decoder_function_text += ''.join(f'#include {item}\n' for item in self._encoder.get_includes())
        # Add 1 to max string length for the null terminator
        decoder_function_text += '\n#define ' + self._encoder._MACRO_NAME + '(text, length) decode_string(text, length)\n'
        decoder_function_text += '\n#define ' + self._encoder._BUFFER_SIZE_DEFINE + ' ' + str(self._max_string_length + 1) + '\n\n'
        decoder_function_text += self._encoder.get_func_def()

        # Update the header file with the decoder function
        with open(self._header.resolve(), "w", encoding='utf-8') as f:
            f.write(decoder_function_text)


def validate_class_has_methods(class_name: str, class_methods: list[str]) -> bool:
    cls_ref = globals()[class_name]
    cls_ins = cls_ref()
    report = []

    for method in class_methods:
        if not hasattr(cls_ins, method):
            report.append(method)

    if len(report) > 0:
        # TODO: specify as debug message
        print(f'Specified encoder [{class_name}] does not define required methods:\n' + ''.join(f'\t{m}' for m in report))
        return False

    return True

def get_derived_classes(cls: Type) -> list[str]:
    valid_classes = []

    classes = [name for name, obj in globals().items() if isinstance(obj, type)]
    methods = [name for name, obj in inspect.getmembers(cls, predicate=inspect.isfunction)]

    filtered_classes = [name for name in classes if issubclass(globals()[name], cls) and globals()[name] is not cls]

    for class_name in filtered_classes:
        if validate_class_has_methods(class_name, methods):
            valid_classes.append(class_name)

    return valid_classes


if __name__ == '__main__':

    arg_parser: argparse.ArgumentParser = argparse.ArgumentParser(description="TODO")
    
    arg_parser.add_argument("root", help="root dir help")
    arg_parser.add_argument("-e", "--encoding", choices=get_derived_classes(Encoder), help="help for encoding")
    
    args = arg_parser.parse_args()

    dir = Path(args.root).resolve()
    files = list(dir.rglob('*.c')) + list(dir.rglob('*.h'))

    if args.encoding is None:
        print('You must choose an encoder. See help -h')
        exit()

    se: StringEncoder = StringEncoder(dir, files, args.encoding)

    se.run()