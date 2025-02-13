# Chainy - Command Chaining Utility

This project implements a utility to chain multiple commands together, where the output of one command is passed as input to the next. This is analogous to Unix pipelines but implemented as a standalone tool.

## Files

- **chainy.c**: Contains the implementation of the command chaining logic.
- **Makefile**: Simplifies the build and test process.

## Functions

### `RemoveSpace`
```c
char* RemoveSpace(char* str);
Removes leading and trailing spaces from a string.
```
### `ParseArgs`
```c
void ParseArgs(char* args, char** arg_res);
Parses a command string into individual commands separated by the pipe (|) symbol.
```
### `create_chai`
```c
void create_chain(char* command, chain_t* chain);
Creates a chain of commands from a parsed command string.
```
### `run_chain`
```c
void run_chain(chain_t* chain);
Executes the chain of commands, passing the output of one command as input to the next.
```

### Testing

- The Makefile includes a set of tests to ensure the functionality of the command chaining utility. The tests cover:

- Checking for forbidden words in the source code (e.g., system, F_SETPIPE_SZ).
- Verifying the presence of the pipe system call.
- Running additional tests using the tester.py script.

### Forbidden words check
cat chainy.c | grep -cE "system" > count || true
cat count | xargs test 0 -eq

cat chainy.c | grep -cE "F_SETPIPE_SZ" > count || true
cat count | xargs test 0 -eq

### Pipe system call check
cat chainy.c | grep -cE "pipe" > count || true
cat count | xargs test 1 -le

### Run additional tests
python3 tester.py


### Build and Run:
```
make        # Build the project
make run    # Run the tests
```
