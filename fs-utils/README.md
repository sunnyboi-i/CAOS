# Directory and File Management Utilities

This project provides two command-line utilities: `mkdir` and `rm`, implemented in C. These utilities mimic the behavior of their Unix counterparts, allowing users to create and remove directories and files.

## Files

- **mkdir.c**: Implements the `mkdir` utility for creating directories.
- **rm.c**: Implements the `rm` utility for removing files and directories.
- **test_mkdir.py**: Python script to test the `mkdir` utility.
- **test_rm.py**: Python script to test the `rm` utility.
- **test_common.py**: Shared utility functions for testing.
- **Makefile**: Simplifies the build and test process.

## Utilities

### `mkdir`
The `mkdir` utility creates directories with optional support for parent directories and custom permissions.

#### Usage
```sh
./mkdir [OPTIONS] <directory>
```
Options
```
-p: Create parent directories as needed.
--mode=<mode>: Set directory permissions (in octal format).
```
### Example
```sh
./mkdir -p --mode=755 /path/to/directory
```
### `rm`

The rm utility removes files and directories. It supports recursive deletion of directories.
### Example
Options
```
-r: Recursively remove directories and their contents.
```

```sh
./rm [OPTIONS] <file_or_directory>
```
Testing

The project includes Python test scripts to verify the functionality of both utilities.

test_mkdir.py

Tests the mkdir utility with various scenarios:

Simple directory creation.
Directory creation with parent directories (-p flag).
Directory creation with custom permissions (--mode flag).
Error handling for missing parent directories.
test_rm.py

Tests the rm utility with various scenarios:

Recursive directory deletion (-r flag).
Multiple file deletion.
Error handling for directory deletion without the -r flag.


### Build and Run

#### Build the utilities:
```
make
```

Run the tests:
```
make test_mkdir
make test_rm
```
