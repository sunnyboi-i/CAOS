# UTF-8 File Handling

This project provides a simple implementation for reading and writing UTF-8 encoded files in C. It includes functions to classify UTF-8 code points, count bytes, and handle file operations.

## Files

- **utf8_file.c**: Contains the implementation of UTF-8 file handling functions.
- **utf8_file.h**: Header file defining the structures and function prototypes.
- **main.c**: Demonstrates the usage of the UTF-8 file handling functions.
- **test.py**: Python script to test the functionality of the C program.
- **Makefile**: Simplifies the build and test process.

## Functions

### `count_byte`
```c
uint8_t count_byte(uint32_t byte);
Determines the number of bytes required to represent a given UTF-8 code point.
```

### `classify`
```c
uint8_t classify(uint32_t cp);
Classifies a Unicode code point into one of the UTF-8 byte length categories.
```
### `utf8_write`
```c
int utf8_write(utf8_file_t* f, const uint32_t* str, size_t count);
Writes a sequence of UTF-8 code points to a file.
```
### `utf8_read`
```c
int utf8_read(utf8_file_t* f, uint32_t* res, size_t count);
Reads a sequence of UTF-8 code points from a file.
```
### `utf8_fromfd`
```c
utf8_file_t* utf8_fromfd(int fd);
Creates a utf8_file_t structure from a file descriptor.
```
## Build the program:
```c 
make
make run
```

### Dependencies

GCC for compiling the C code. \
Python 3 for running the test script.

