# File Allocator

This project implements a file-based memory allocator in C. It allows for the allocation and release of fixed-size memory pages backed by a file, providing persistent storage for data. The allocator uses memory-mapped files (`mmap`) for efficient access and management.

## Files

- **falloc.c**: Contains the implementation of the file allocator.
- **falloc.h**: Header file defining the structures and function prototypes for the allocator.
- **tester.c**: Test program to verify the functionality of the file allocator.
- **Makefile**: Simplifies the build and test process.

## Functions

### `falloc_init`
```c
void falloc_init(file_allocator_t* allocator, const char* filepath, uint64_t allowed_page_count);
Initializes the file allocator with a specified file and maximum number of pages.
```

### `falloc_destroy`
```c
void falloc_destroy(file_allocator_t* allocator);
Destroys the file allocator, unmapping the file and releasing resources.
```
### `falloc_acquire_page`
```c
void* falloc_acquire_page(file_allocator_t* allocator);
Allocates a new page from the file and returns its address.
```

### `falloc_release_page`
```c
void falloc_release_page(file_allocator_t* allocator, void** addr);
Releases a previously allocated page, making it available for future allocations.
```
### Testing

The tester.c file includes a suite of tests to verify the functionality of the file allocator. The tests cover:

- Initialization and destruction of the allocator.
- Allocation and release of pages.
- Persistence of data across allocator sessions.
- Edge cases, such as attempting to allocate more pages than allowed.

```c
test_init_destroy(); // Tests initialization and destruction.
test_acquire();      // Tests page allocation.
test_release();      // Tests page release.
test_scenario();     // Tests a complex usage scenario.
```


### Build and run:
Build:
```c
make 
```
Run:
```c
make run 
```

