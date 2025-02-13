# MiniGrep

This project implements a simplified version of the `grep` utility in C. It searches for lines matching a regular expression in files within a directory and its subdirectories. The utility uses the PCRE (Perl Compatible Regular Expressions) library for regex matching.

## Files

- **minigrep.c**: Contains the implementation of the `minigrep` utility.
- **test_minigrep.py**: Python script to test the functionality of `minigrep`.
- **Makefile**: Simplifies the build and test process.

## Features

- Recursively searches directories for files.
- Matches lines in files against a provided regular expression.
- Prints matching lines in the format: `<filepath>:<line_number>: <line_content>`.
- Handles invalid regular expressions gracefully.

## Usage

1. **Build the project**:
   ```sh
   make
   ```
Run the utility:
```sh
./minigrep <regex> <directory>
```
- <regex>: The regular expression to search for.
- <directory>: The directory to search in.
```c
./minigrep "const" ./src
```
