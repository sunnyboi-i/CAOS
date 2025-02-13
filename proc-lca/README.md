# Lowest Common Ancestor (LCA) in Process Trees

This project implements a utility to find the Lowest Common Ancestor (LCA) of two processes in a process tree. The LCA is the nearest common ancestor of two processes in the hierarchical structure of the process tree.

## Files

- **lca.c**: Contains the implementation of the LCA algorithm for process trees.
- **lca.h**: Header file defining the function prototype for the LCA utility.
- **tester.c**: Test program to verify the correctness of the LCA implementation.
- **Makefile**: Simplifies the build and test process.

## Functions

### `find_lca`
```c
pid_t find_lca(pid_t x, pid_t y);
Finds the Lowest Common Ancestor (LCA) of two processes with IDs `x` and `y`.
```

### `GetPpid`
```c
int GetPpid(int fd);
Reads the parent process ID (PPID) from a process status file.
```

### `MakePath`
```c
char* MakePath(pid_t x);
Constructs the file path to the status file of a process with ID `x`.
```

### `BuildParents`
```c
int* BuildParents(pid_t pid, int* counter);
Builds an array of ancestor process IDs for a given process ID `pid`.
```

### Testing
The tester.c file includes a suite of tests to verify the correctness of the LCA implementation. The tests cover:

- Finding the LCA of a process with itself.
- Finding the LCA of a process and its parent.
- Finding the LCA of sibling processes.
- Finding the LCA in randomly generated process trees.

```c
test_same();      // Tests LCA of a process with itself.
test_parent();    // Tests LCA of a process and its parent.
test_siblings();  // Tests LCA of sibling processes.
test_random();    // Tests LCA in randomly generated process trees.
```


### Build and run:
```
make 
make run
```
